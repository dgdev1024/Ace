/**
 * @file    Ace/System/RingBuffer.hpp
 * @brief   Provides a container class for storing data in a circular queue.
 */

#pragma once
#include <Ace/Common.hpp>

namespace ace
{

    /**
     * @brief   A container class used for storing data in a multiple-producer,
     *          single-consumer (MPSC) circular queue.
     * 
     * @tparam  T           The type of data stored in this queue.
     * @tparam  Capacity    The maximum capacity of this queue.
     */
    template <typename T, std::size_t Capacity>
    class RingBuffer final
    {
    private:
        //
        // `(Capacity & (Capacity - 1)) == 0`
        //
        // The bitwise representation of a power of two is always guarnteed to
        // have exactly ONE bit set - the remaining bits after it are clear.
        // Subtracting one from a power of two will result in an integer whose
        // bitwise representation is guarnteed to have ALL of its bits set.
        //
        // Example:
        // - `2 pow 3` = `8` = `0b1000`
        // - `(2 pow 3) - 1 = `7` = `0b0111`
        // - '8 AND 7` = `0b1000 AND 0b0111` = `0b0000` = `0`
        //
        // Knowing that a bitwise AND only evaulates to `true` if both bits
        // checked are set, we can check for a power of two by performing a
        // bitwise AND between a number and that number minus one, and checking
        // if the result is zero.
        //
        static_assert((Capacity & (Capacity - 1)) == 0,
            "'ace::RingBuffer' capacity must be a power of two.");

    private:

        /**
         * @brief   A helper structure representing a single element in this
         *          ring buffer.
         */
        struct Cell
        {
            std::atomic<std::size_t>    mSequenceNumber;    ///< @brief The cell's sequence number, used to indicate if the cell is free, ready to read, or is still owned by a producer.
            T                           mData;              ///< @brief The data contained in this cell, if any.
        };

        /**
         * @brief   A mask of this ring buffer's capacity.
         */
        static constexpr std::size_t MASK = Capacity - 1;

    public:

        /**
         * @brief   The default constructor initializes the circular queue's
         *          cells' atomically-stored sequence numbers.
         */
        explicit RingBuffer () noexcept
        {
            for (std::size_t i = 0; i < Capacity; ++i)
            {
                mBuffer[i].mSequenceNumber.store(i,
                    std::memory_order_relaxed);
            }
        }

        /**
         * @brief   Attempts to push a new item into the circular queue.
         * 
         * @param   pItem   A handle to the item to be enqueued.
         * 
         * @return  `true` if the item is enqueued; `false` if there was no room
         *          available in the queue.
         */
        bool enqueue (
            const T&    pItem
        ) noexcept
        {

            // `std::atomic::fetch_add` retrieves the value of an atomic 
            // variable, then attempts to add to that variable. The value
            // retrieved is the atomic's value before the add - in this case,
            // the current position of the circular queue's head element.
            //
            // Use that position to index that head element.
            std::size_t lPosition   = mHead.fetch_add(1, std::memory_order_acquire);
            Cell&       lCell       = mBuffer[lPosition & MASK];

            // Check the sequence number of the cell. If it and the
            // above-retrieved head position are not equal, then either...
            // - ...another producer has a hold on the slot.
            // - ...the circular queue is full.
            std::size_t lSequenceNumber = lCell.mSequenceNumber.load(
                std::memory_order_acquire
            );
            while (lSequenceNumber != lPosition)
            {
                if (lPosition - lSequenceNumber >= Capacity)
                {
                    return false;
                }

                lSequenceNumber = lCell.mSequenceNumber.load(
                    std::memory_order_acquire
                );
            }

            // Emplace the item, then update its sequence number.
            lCell.mData = pItem;
            lCell.mSequenceNumber.store(lPosition + 1,
                std::memory_order_release);

            return true;

        }

        /**
         * @brief   Attempts to pop an item from the circular queue.
         * 
         * @return  An `std::optional` which contains the dequeued item if one
         *          was popped.
         */
        std::optional<T> dequeue () noexcept
        {

            // Index the appropriate cell.
            std::size_t lPosition   = mTail.load(std::memory_order_relaxed);
            Cell&       lCell       = mBuffer[lPosition & MASK];

            // Check to see if the cell has data that is ready to be retrieved.
            std::size_t lSequenceNumber = lCell.mSequenceNumber.load(
                std::memory_order_acquire
            );
            if (lSequenceNumber != lPosition + 1)
            {
                return std::nullopt;
            }

            // Move the item out of the queue, then update that cell's sequence
            // number and the tail pointer.
            T lItem = std::move(lCell.mData);
            lCell.mSequenceNumber.store(lPosition = Capacity,
                std::memory_order_release);
            mTail.store(lPosition + 1, std::memory_order_relaxed);

            return lItem;

        }

    private:
                    std::array<Cell, Capacity>  mBuffer;        ///< @brief Contains the circular queue's data, and their respective sequence numbers.
        alignas(64) std::atomic<std::size_t>    mHead { 0 };    ///< @brief The circular queue's head element, atomically incremented by producers to reserve a slot index.
        alignas(64) std::atomic<std::size_t>    mTail { 0 };    ///< @brief The circular queue's tail element, modified by the queue's single consumer to track which slot to read next.

    };

}
