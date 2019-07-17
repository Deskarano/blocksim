#ifndef BLOCKSIM_SHA256_H
#define BLOCKSIM_SHA256_H

void SHA256(unsigned char *data, unsigned int len, unsigned char target[32]);

void print_hash(unsigned char *hash);

#endif //BLOCKSIM_SHA256_H
