/**
 * \file ring_buffer.h
 * \brief Ring Buffer for MCU
 * \date 2024-04
 * \author Nicolas Boutin
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "buffer/buffer.h"

// --- Public types

typedef uint16_t RBUF_size_t;

typedef struct RBUF_s
{
  uint8_t *data;           /*!< Buffer data */
  RBUF_size_t write_index; /*!< Write index */
  RBUF_size_t read_index;  /*!< Read index */
  RBUF_size_t size;        /*!< Buffer size */
} RBUF_t;

// --- Public functions

/**
 * \brief Initialize empty buffer
 * \param buffer Buffer to initialize
 * \param data Buffer data
 * \param size Buffer size
 * \details write_index = 0, read_index = 0
 */
void RBUF_InitEmpty(RBUF_t *buffer, uint8_t *data, uint16_t size);

/**
 * \brief Check for empty buffer
 * \param buffer Buffer to check
 * \return true if buffer is empty, false otherwise
 */
bool RBUF_IsEmpty(const RBUF_t *buffer);

/**
 * \brief Check for full buffer
 * \param buffer Buffer to check
 * \return true if buffer is full, false otherwise
 */
bool RBUF_IsFull(const RBUF_t *buffer);

/**
 * \brief Get free space in buffer
 * \param buffer Buffer to check
 * \return Free space in buffer
 */
RBUF_size_t RBUF_GetFreeSize(const RBUF_t *buffer);

/**
 * \brief Get used space in buffer
 * \details write_index - read_index
 * \param buffer Buffer to check
 * \return Used space in buffer
 */
RBUF_size_t RBUF_GetUsedSize(const RBUF_t *buffer);

/**
 * \brief Get write index
 * \param buffer Buffer to check
 * \return write index
 */
RBUF_size_t RBUF_GetWriteIndex(const RBUF_t *buffer);

/**
 * \brief Write uint8_t to buffer
 * \param buffer Buffer to write to
 * \param data Data to write
 * \return true if data was written, false otherwise
 */
bool RBUF_WriteUint8(RBUF_t *buffer, uint8_t data);

/**
 * \brief Write uint16_t to buffer
 * \param buffer Buffer to write to
 * \param data Data to write
 * \return true if data was written, false otherwise
 */
bool RBUF_WriteUint16(RBUF_t *buffer, uint16_t data);

/**
 * \brief Write uint32_t to buffer
 * \param buffer Buffer to write to
 * \param data Data to write
 * \return true if data was written, false otherwise
 */
bool RBUF_WriteUint32(RBUF_t *buffer, uint32_t data);

/**
 * \brief Write string to buffer
 * \param buffer Buffer to write to
 * \param data String to write
 * \param size Size of string to write
 * \return true if data was written, false otherwise
 */
bool RBUF_WriteString(RBUF_t *buffer, const char *data, RBUF_size_t size);

/**
 * \brief Write to buffer
 * \param rbuf_dst Destination buffer
 * \param buf_src Source buffer
 * \param size Size to copy
 * \return true if requested size was copied, false otherwise
 * \details If size is greater than free space in buffer or available data to read, nothing is copied
 */
bool RBUF_WriteCopy(RBUF_t *rbuf_dst, BUF_t *buf_src, RBUF_size_t size);

/**
 * \brief Read uint8_t from buffer
 * \param buffer Buffer to read from
 * \return value read
 */
uint8_t RBUF_ReadUint8(RBUF_t *buffer);

/**
 * \brief Read requested size from buffer, if not possible does nothing
 * \param buf_dst Destination buffer
 * \param rbuf_src Source buffer
 * \param size Size to read
 * \return true if requested size was read, false otherwise
 */
bool RBUF_ReadCopyBlock(BUF_t *buf_dst, RBUF_t *rbuf_src, RBUF_size_t size);

/**
 * \brief Read request size from buffer, if not possible return what can be read
 * \param buf_dst Destination buffer
 * \param Source Source buffer
 * \param size Size to read
 * \return Size read
 */
RBUF_size_t RBUF_ReadCopyRaw(BUF_t *buf_dst, RBUF_t *rbuf_src, RBUF_size_t size);
