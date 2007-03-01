#include <alcor/act/shrimp_server_t.hpp>

int main() {
	all::act::shrimp_server_t m_shrimp;

	m_shrimp.run_async();
	getchar();
	
	m_shrimp.stop();
	getchar();
}


