
#pragma once


class uart_stdio_blocking {
public:
	uart_stdio_blocking();
};

namespace _priv_uart_stdio_non_blocking {
	class queue_view;
	void init(char* buf_begin, char* buf_end);
}
template<int BUF_SIZE = 100>
class uart_stdio_non_blocking {
public:
	friend class _priv_uart_stdio_non_blocking::queue_view;
	uart_stdio_non_blocking() {
		_priv_uart_stdio_non_blocking::init(buf, buf + BUF_SIZE);
	}
private:
	char buf[BUF_SIZE];
};
