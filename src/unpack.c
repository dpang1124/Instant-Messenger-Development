#include <stdlib.h>
#include <string.h>

#include "serialize.h"

/* Unpack the given packet into the buffer message.  You can assume
 * that packed points to a packet buffer large enough to hold the
 * packet described therein, but you cannot assume that the packet is
 * otherwise valid.  You can assume that message points to a character
 * buffer large enough to store the unpacked packet, if it is a valid
 * packet.
 *
 * Returns the packet type that was unpacked, or -1 if it is invalid.
 */
int unpack(char *message, void *packed) {
    return 0;
}

/* Unpack the given packed packet into the given statistics structure.
 * You can assume that packed points to a packet buffer large enough to
 * hold the statistics packet, but you cannot assume that it is
 * otherwise valid.  You can assume that statistics points to a
 * statistics structure.
 *
 * Returns the packet type that was unpacked, or -1 if it is invalid.
 */
int unpack_statistics(struct statistics *statistics, void *packed) {
    return 0;
}
