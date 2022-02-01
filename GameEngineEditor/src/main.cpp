#include <iostream>
#include <memory>

#include <GameEngineCore/Application.hpp>

class MyApp : public GameEngine::Application {
	virtual void on_update() override {

	}


};

int main() {
	
	auto myApp = std::make_unique<MyApp>();
	int returnCode = myApp->start(1024, 768, "Shinzo wo Sasageyo!");
	
	return returnCode;
	
}