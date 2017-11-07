#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

#define RING_SIZE  (16)


struct RingBuffer {
    unsigned int  tail;
    unsigned int  head;
    unsigned char data[RING_SIZE];
};

char ringBufGetChar (struct RingBuffer* apBuffer);
void ringBufPutChar (struct RingBuffer* apBuffer, const char c);

#endif /* RING_BUFFER_H_ */
