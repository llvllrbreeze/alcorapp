#include <alcor/act/pololu_ctrl_server_t.hpp>

int main() {
	all::act::pololu_ctrl_server_t pololu;

	pololu.run_async();

	getchar();

	pololu.stop();
	getchar();
}
