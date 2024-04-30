#pragma once

#define OS_LINUX

#define DISALLOW_COPY(className) \
	className(const className &) = delete; \
	className &operator = (const className &) = delete;

#define DISALLOW_MOVE(className) \
	className(className &&) = delete; \
	className &operator = (className &&) = delete;

#define DISALLOW_COPY_AND_MOVE(className) \
	DISALLOW_COPY(className);             \
	DISALLOW_MOVE(className);