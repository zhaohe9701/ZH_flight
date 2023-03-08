/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-03-01 23:34:31
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-01 23:55:56
 * @FilePath: \ZH_FLIGHT\Sys\OS\os.cpp
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#include "os.h"
#include "cmsis_os2.c"

osStatus_t osMessageQueueOverwrite (osMessageQueueId_t mq_id, const void *msg_ptr) {
  QueueHandle_t hQueue = (QueueHandle_t)mq_id;
  osStatus_t stat;
  BaseType_t yield;

  stat = osOK;

  if (IS_IRQ()) {
    if ((hQueue == NULL) || (msg_ptr == NULL)) {
      stat = osErrorParameter;
    }
    else {
      yield = pdFALSE;

      if (xQueueOverwriteFromISR (hQueue, msg_ptr, &yield) != pdTRUE) {
        stat = osErrorResource;
      } else {
        portYIELD_FROM_ISR (yield);
      }
    }
  }
  else {
    if ((hQueue == NULL) || (msg_ptr == NULL)) {
      stat = osErrorParameter;
    }
    else {
      if (xQueueOverwrite (hQueue, msg_ptr) != pdPASS) {
        stat = osErrorResource;
      }
    }
  }

  return (stat);
}

osStatus_t osMessageQueuePeek (osMessageQueueId_t mq_id, void *msg_ptr, uint8_t *msg_prio, uint32_t timeout) {
  QueueHandle_t hQueue = (QueueHandle_t)mq_id;
  osStatus_t stat;
  BaseType_t yield;

  (void)msg_prio; /* Message priority is ignored */

  stat = osOK;

  if (IS_IRQ()) {
    if ((hQueue == NULL) || (msg_ptr == NULL) || (timeout != 0U)) {
      stat = osErrorParameter;
    }
    else {
      yield = pdFALSE;

      if (xQueuePeekFromISR (hQueue, msg_ptr) != pdPASS) {
        stat = osErrorResource;
      } else {
        portYIELD_FROM_ISR (yield);
      }
    }
  }
  else {
    if ((hQueue == NULL) || (msg_ptr == NULL)) {
      stat = osErrorParameter;
    }
    else {
      if (xQueuePeek (hQueue, msg_ptr, (TickType_t)timeout) != pdPASS) {
        if (timeout != 0U) {
          stat = osErrorTimeout;
        } else {
          stat = osErrorResource;
        }
      }
    }
  }

  return (stat);
}