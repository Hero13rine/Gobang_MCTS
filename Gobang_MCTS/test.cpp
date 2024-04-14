#include <torch/torch.h>
#include <iostream>
int main() {
	torch::Tensor data = torch::rand({ 3, 3 });
	std::cout << data << std::endl;
	std::cout << torch::cuda::is_available() << std::endl;
	return 0;
}