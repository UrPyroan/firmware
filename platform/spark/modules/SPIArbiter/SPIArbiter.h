/*
 * Copyright 2016 BrewPi/Matthew McGowan.
 *
 * This file is part of BrewPi.
 *
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BrewPi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */


#pragma once

#include "Platform.h"
#include <functional>

template <class T>
class GuardedResource {
public:

    inline os_mutex_recursive_t _get_mutex() {
        return (static_cast<T*>(this))->get_mutex();
    }

    inline bool try_lock()
    {
#if 0 // PLATFORM_THREADING
        return !os_mutex_recursive_trylock(_get_mutex());
#else
        return true;
#endif
    }

    inline void lock()
    {
#if 0 // PLATFORM_THREADING
        os_mutex_recursive_lock(_get_mutex());
#endif
    }

    inline void unlock()
    {
#if 0 // PLATFORM_THREADING
        os_mutex_recursive_unlock(_get_mutex());
#endif
    }

};

class SPIConfiguration
{
public:
    SPIConfiguration() : mode_(SPI_MODE0), bitOrder_(MSBFIRST), clockDivider_(SPI_CLOCK_DIV128), ss_pin_(UINT16_MAX){}
    ~SPIConfiguration() = default;

protected:
    uint8_t mode_;
    uint8_t bitOrder_;
    uint8_t clockDivider_;
    uint16_t ss_pin_;

public:

    inline uint8_t getMode() const { return mode_; }
    inline uint8_t getBitOrder() const { return bitOrder_; }
    inline uint8_t getClockDivider() const { return clockDivider_; }
    inline uint16_t getSSPin() const { return ss_pin_; }
};

class SPIArbiter : private SPIConfiguration, public GuardedResource<SPIArbiter>
{
    SPIConfiguration* current_;
    SPIClass& spi_;
    os_mutex_recursive_t mutex_;


    void apply(SPIConfiguration& client);
    // os_mutex_recursive_t get_mutex() { return mutex_; }

    friend class GuardedResource<SPIArbiter>;

public:

    SPIArbiter(SPIClass& spi) : current_(nullptr), spi_(spi), mutex_(nullptr) {
        // os_mutex_recursive_create(&mutex_);
    }

    ~SPIArbiter() {
        // os_mutex_recursive_destroy(&mutex_);
    }

    inline bool try_begin(SPIConfiguration& client) {
        if (isClient(client) || try_lock()) {
            current_ = &client;
            apply(client);
        }
        return isClient(client);
    }

    inline void begin(SPIConfiguration& client) {
        lock();
        current_ = &client;
        apply(client);
    }

    inline void end(SPIConfiguration& client) {
        if (isClient(client)) {
            current_ = nullptr;
            digitalWrite(ss_pin_, HIGH); // unselect client
            ss_pin_ = UINT16_MAX; // set ss_pin to UINT16_MAX to indicate no active client
            unlock();
        }
    }

    /*
     * transfer locks the SPI arbiter, performs the transfer of a single byte and unlocks it again.
     * it also asserts the SS PIN
     */
    inline uint8_t transfer(SPIConfiguration& client, uint8_t data) {
        if (isClient(client)) {
            return spi_.transfer(data);
        }
        return 0;
    }

    inline void transfer(SPIConfiguration& client, void* tx_buffer, void* rx_buffer, size_t length, wiring_spi_dma_transfercomplete_callback_t user_callback) {
        if (isClient(client)) {
            spi_.transfer(tx_buffer, rx_buffer, length, user_callback);
        }
        // todo - should we independently track that DMA is in progress?
    }

    inline void transferCancel(SPIConfiguration& client) {
        if (isClient(client)) {
            spi_.transferCancel();
        }
    }


    inline bool isClient(SPIConfiguration& client) {
        return &client==current_;
    }
};

/**
 *
 */
class SPIUser : public SPIConfiguration
{
    SPIArbiter& spi_;

public:

    SPIUser(SPIArbiter& spi) : spi_(spi) {}

    inline void begin() {
        spi_.begin(*this);
    }

    inline void begin(uint16_t ss_pin) {
        ss_pin_ = ss_pin;
        spi_.begin(*this);
    }

    inline void end() {
        spi_.end(*this);
    }

    inline void setBitOrder(uint8_t bitOrder) {
        bitOrder_ = bitOrder;
    }

    inline void setDataMode(uint8_t dataMode) {
        mode_ = dataMode;
    }

    /**
     * Sets the clock speed as a divider relative to the clock divider reference.
     * @param divider SPI_CLOCK_DIVx where x is a power of 2 from 2 to 256.
     */
    inline void setClockDivider(uint8_t divider) {
        clockDivider_ = divider;
    }

    inline byte transfer(byte _data) {
        return spi_.transfer(*this, _data);
    }

    inline void transfer(void* tx_buffer, void* rx_buffer, size_t length, wiring_spi_dma_transfercomplete_callback_t user_callback) {
        spi_.transfer(*this, tx_buffer, rx_buffer, length, user_callback);
    }

    inline void transferCancel() {
        spi_.transferCancel(*this);
    }
};

extern SPIArbiter GlobalSPIArbiter;
