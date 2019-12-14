#include <iostream>
#include <SFML/Graphics.hpp>
#include "Server.h"

using namespace sf;

int main() {
	//RenderWindow window({ 600, 600 }, "circles", Style::Close | Style::Titlebar);
	constexpr int PORT = 2000;
	Server server(PORT);

	server.runServer();

	/*while (window.isOpen()) {
		Event e;
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed)
				window.close();
		}
			
		window.clear();
		window.display();
	}*/
}