/**
 * Copyright (c) 2011-2015 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * libbitcoin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include <bitcoin/database/primitives/record_list.hpp>

#include <cstdint>
#include <bitcoin/bitcoin.hpp>
#include <bitcoin/database/memory/memory.hpp>

namespace libbitcoin {
namespace database {

record_list::record_list(record_manager& manager)
  : manager_(manager)
{
    static_assert(sizeof(array_index) == sizeof(uint32_t),
        "array_index incorrect size");
}

array_index record_list::create()
{
    // Insert new record with null next value.
    return insert(empty);
}

array_index record_list::insert(array_index index)
{
    // Create new record and return its index.
    auto new_index = manager_.new_records(1);
    const auto memory = manager_.get(new_index);
    auto serial = make_serializer(REMAP_ADDRESS(memory));
    //*************************************************************************
    serial.template write_little_endian<array_index>(index);
    //*************************************************************************
    return new_index;
}

array_index record_list::next(array_index index) const
{
    const auto memory = manager_.get(index);
    const auto next_address = REMAP_ADDRESS(memory);
    //*************************************************************************
    return from_little_endian_unsafe<array_index>(next_address);
    //*************************************************************************
}

const memory_ptr record_list::get(array_index index) const
{
    auto memory = manager_.get(index);
    REMAP_INCREMENT(memory, sizeof(array_index));
    return memory;
}

} // namespace database
} // namespace libbitcoin
