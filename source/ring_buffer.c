/**
 * \file ring_buffer.c
 * \brief Ring Buffer for MCU
 * \date 2024-04
 * \author Nicolas Boutin
 * \details
 * Full and empty state of ring buffer looks identical when write and read pointer are equal.
 * To differentiate them:
 * - full state: write  + 1 = read
 * - empty state: write == read
 */
#include <string.h>

#include "ring_buffer/ring_buffer.h"

// --- Private functions

static RBUF_size_t Rbuf_Min(RBUF_size_t a, RBUF_size_t b);
static bool Rbuf_WillWriteRollover(const RBUF_t *buffer, RBUF_size_t size);
static bool Rbuf_WillReadRollover(const RBUF_t *buffer, RBUF_size_t size);
static bool Rbuf_is_memory_overlapping(const void *dest, const void *buf_src, size_t length);

// --- Public functions

void RBUF_InitEmpty(RBUF_t *buffer, uint8_t *data, uint16_t size)
{
  if (buffer != NULL)
  {
    buffer->data = data;
    buffer->size = size;
    buffer->read_index = 0;
    buffer->write_index = 0;
  }
}

bool RBUF_IsEmpty(const RBUF_t *buffer)
{
  bool empty = false;

  if (buffer != NULL)
  {
    if (buffer->write_index == buffer->read_index)
    {
      empty = true;
    }
  }
  return empty;
}

bool RBUF_IsFull(const RBUF_t *buffer)
{
  bool full = false;

  if ((buffer != NULL) && (buffer->size > 0U))
  {
    full = ((buffer->write_index + 1) % buffer->size) == buffer->read_index;
  }
  return full;
}

RBUF_size_t RBUF_GetFreeSize(const RBUF_t *buffer)
{
  RBUF_size_t free_size = 0U;

  if (buffer != NULL)
  {
    if (buffer->write_index >= buffer->read_index) // No rollover
    {
      free_size = buffer->size - buffer->write_index + buffer->read_index;
    }
    else // Rollover
    {
      free_size = buffer->read_index - buffer->write_index;
    }
    free_size -= 1U; // One byte is always reserved to differentiate between full and empty state
  }
  return free_size;
}

RBUF_size_t RBUF_GetUsedSize(const RBUF_t *buffer)
{
  RBUF_size_t used_size = 0U;

  if (buffer != NULL)
  {
    if (buffer->write_index >= buffer->read_index) // No rollover
    {
      used_size = buffer->write_index - buffer->read_index;
    }
    else // Rollover
    {
      used_size = buffer->size - buffer->read_index + buffer->write_index;
    }
  }
  return used_size;
}

RBUF_size_t RBUF_GetWriteIndex(const RBUF_t *buffer)
{
  RBUF_size_t write_index = 0U;

  if (buffer != NULL)
  {
    write_index = buffer->write_index;
  }
  return write_index;
}

bool RBUF_WriteUint8(RBUF_t *buffer, uint8_t data)
{
  bool written = false;

  if ((RBUF_GetFreeSize(buffer) >= 1U) && (buffer != NULL) && (buffer->data != NULL))
  {
    if (Rbuf_WillWriteRollover(buffer, 1U) == false)
    {
      buffer->data[buffer->write_index] = data;
      buffer->write_index++;
    }
    else
    {
      /* Write will rollover */
      buffer->data[buffer->write_index] = data;
      buffer->write_index = 0U;
    }
    written = true;
  }
  return written;
}

bool RBUF_WriteUint16(RBUF_t *buffer, uint16_t data)
{
  bool written = true;

  if (RBUF_GetFreeSize(buffer) >= 2U)
  {
    written &= RBUF_WriteUint8(buffer, (uint8_t)(data >> 8U));
    written &= RBUF_WriteUint8(buffer, (uint8_t)(data & 0x00FFU));
  }
  else
  {
    written = false;
  }
  return written;
}

bool RBUF_WriteUint32(RBUF_t *buffer, uint32_t data)
{
  bool written = true;

  if (RBUF_GetFreeSize(buffer) >= 4U)
  {
    written &= RBUF_WriteUint8(buffer, (uint8_t)(data >> 24U));
    written &= RBUF_WriteUint8(buffer, (uint8_t)(data >> 16U));
    written &= RBUF_WriteUint8(buffer, (uint8_t)(data >> 8U));
    written &= RBUF_WriteUint8(buffer, (uint8_t)(data & 0x000000FFU));
  }
  else
  {
    written = false;
  }
  return written;
}

bool RBUF_WriteString(RBUF_t *buffer, const char *data, RBUF_size_t size)
{
  bool written = false;

  if ((RBUF_GetFreeSize(buffer) >= size) && (buffer != NULL) && (buffer->data != NULL) && (data != NULL) && (Rbuf_is_memory_overlapping(buffer->data, data, size) == false))
  {
    if (Rbuf_WillWriteRollover(buffer, size) == false)
    {
      memcpy(&buffer->data[buffer->write_index], data, size);
      buffer->write_index += size;
    }
    else
    {
      /* Write will rollover */
      RBUF_size_t size1 = buffer->size - buffer->write_index;
      RBUF_size_t size2 = size - size1;

      memcpy(&buffer->data[buffer->write_index], data, size1);
      memcpy(&buffer->data[0], &data[size1], size2);
      buffer->write_index = size2;
    }
    written = true;
  }
  return written;
}

bool RBUF_WriteCopy(RBUF_t *rbuf_dst, BUF_t *buf_src, RBUF_size_t size)
{
  bool written = false;
  BUF_size_t dst_free_space = RBUF_GetFreeSize(rbuf_dst);
  BUF_size_t src_to_read_count = BUF_GetToReadCount(buf_src);

  if ((dst_free_space >= src_to_read_count) && (src_to_read_count >= size))
  {
    if ((rbuf_dst != NULL) && (buf_src != NULL) && (rbuf_dst->data != NULL) && (buf_src->data != NULL) && (Rbuf_is_memory_overlapping(rbuf_dst->data, buf_src->data, size) == false))
    {
      if (Rbuf_WillWriteRollover(rbuf_dst, size) == false)
      {
        memcpy(&rbuf_dst->data[rbuf_dst->write_index], &buf_src->data[buf_src->read_index], size);
        rbuf_dst->write_index += size;
        buf_src->read_index += size;

        written = true;
      }
      else
      {
        /* Write will rollover */
        RBUF_size_t size1 = rbuf_dst->size - rbuf_dst->write_index;
        RBUF_size_t size2 = size - size1;

        memcpy(&rbuf_dst->data[rbuf_dst->write_index], &buf_src->data[buf_src->read_index], size1);
        buf_src->read_index += size1;

        memcpy(&rbuf_dst->data[0], &buf_src->data[buf_src->read_index], size2);
        rbuf_dst->write_index = size2;
        buf_src->read_index += size2;

        written = true;
      }
    }
  }
  return written;
}

uint8_t RBUF_ReadUint8(RBUF_t *buffer)
{
  uint8_t data = 0U;

  if ((RBUF_GetUsedSize(buffer) >= 1U) && (buffer != NULL) && (buffer->data != NULL) && (buffer->size > 0U))
  {
    data = buffer->data[buffer->read_index];
    buffer->read_index = (buffer->read_index + 1U) % buffer->size;
  }
  return data;
}

bool RBUF_ReadCopyBlock(BUF_t *buf_dst, RBUF_t *rbuf_src, RBUF_size_t size)
{
  bool read = false;
  RBUF_size_t src_to_read = RBUF_GetUsedSize(rbuf_src);
  BUF_size_t dst_free_space = BUF_GetFreeSize(buf_dst);

  if ((buf_dst != NULL) && (buf_dst->data != NULL) && (rbuf_src != NULL) && (rbuf_src->data != NULL))
  {
    if ((dst_free_space >= src_to_read) && (src_to_read >= size)) /*!< check enough data to be read */
    {
      if (Rbuf_WillReadRollover(rbuf_src, size) == false) // No rollover on read
      {
        memcpy(&buf_dst->data[buf_dst->write_index], &rbuf_src->data[rbuf_src->read_index], size);
        rbuf_src->read_index += size;
        buf_dst->write_index += size;
      }
      else // Rollover on read
      {
        RBUF_size_t size1 = rbuf_src->size - rbuf_src->read_index;
        RBUF_size_t size2 = size - size1;

        memcpy(&buf_dst->data[buf_dst->write_index], &rbuf_src->data[rbuf_src->read_index], size1);
        buf_dst->write_index += size1;

        memcpy(&buf_dst->data[buf_dst->write_index], &rbuf_src->data[0], size2);
        rbuf_src->read_index = size2;
        buf_dst->write_index += size2;
      }
      read = true;
    }
  }
  return read;
}

RBUF_size_t RBUF_ReadCopyRaw(BUF_t *buf_dst, RBUF_t *rbuf_src, RBUF_size_t size)
{
  RBUF_size_t read = 0U;
  RBUF_size_t src_to_read = RBUF_GetUsedSize(rbuf_src);
  BUF_size_t dst_free_space = BUF_GetFreeSize(buf_dst);
  RBUF_size_t to_read = Rbuf_Min(src_to_read, dst_free_space);
  to_read = Rbuf_Min(to_read, size);

  if ((buf_dst != NULL) && (buf_dst->data != NULL) && (rbuf_src != NULL) && (rbuf_src->data != NULL) && (Rbuf_is_memory_overlapping(buf_dst->data, rbuf_src->data, size) == false))
  {
    if (Rbuf_WillReadRollover(rbuf_src, to_read) == false) // No rollover on read
    {
      memcpy(&buf_dst->data[buf_dst->write_index], &rbuf_src->data[rbuf_src->read_index], to_read);
      rbuf_src->read_index += to_read;
      buf_dst->write_index += to_read;
    }
    else // Rollover on read
    {
      RBUF_size_t size1 = rbuf_src->size - rbuf_src->read_index;
      RBUF_size_t size2 = to_read - size1;

      memcpy(&buf_dst->data[buf_dst->write_index], &rbuf_src->data[rbuf_src->read_index], size1);
      buf_dst->write_index += size1;

      memcpy(&buf_dst->data[buf_dst->write_index], &rbuf_src->data[0], size2);
      rbuf_src->read_index = size2;
      buf_dst->write_index += size2;
    }
    read = to_read;
  }
  return read;
}

// --- Private functions

static RBUF_size_t Rbuf_Min(RBUF_size_t a, RBUF_size_t b)
{
  return (a < b) ? a : b;
}

static bool Rbuf_WillWriteRollover(const RBUF_t *buffer, RBUF_size_t size)
{
  bool is_rollover = false;
  if (buffer != NULL)
  {
    is_rollover = ((buffer->write_index + size) >= buffer->size);
  }
  return is_rollover;
}

/**
 * \brief Check if the buffer is going to rollover for the requested size
 * \param buffer buffer to check rollover on
 * \param size size to check rollover for
 * \return true if the buffer is going to rollover, false otherwise
 */
static bool Rbuf_WillReadRollover(const RBUF_t *buffer, RBUF_size_t size)
{
  bool is_rollover = false;
  if (buffer != NULL)
  {
    is_rollover = ((buffer->read_index + size) > buffer->size);
  }
  return is_rollover;
}

static bool Rbuf_is_memory_overlapping(const void *dest, const void *src, size_t length)
{
  bool is_overlapping = false;

  if ((dest != NULL) && (src != NULL))
  {
    uintptr_t dest_start = (uintptr_t)dest;
    uintptr_t dest_end = dest_start + length;
    uintptr_t src_start = (uintptr_t)src;
    uintptr_t src_end = src_start + length;

    is_overlapping =
        ((dest_start >= src_start) && (dest_start < src_end)) || ((src_start >= dest_start) && (src_start < dest_end));
  }
  return is_overlapping;
}
