#pragma once

/**
 * @brief 检查条件，如果条件为true，则抛出错误。
 *
 * @param condition 检查的条件
 * @param message 如果条件为true，则要引发的错误消息。
 */
void errorif(bool condition, const char *message);