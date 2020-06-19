#pragma once
//Singleton class
class Assets {
private:
	Assets()
	{};

public:

	static Assets& Get() {
		static Assets assetsInstance;
		return assetsInstance;
	}
	Assets(const Assets&) = delete;
	

	sf::Font font;
	std::map<std::string, sf::Texture> textures;
	sf::Image icon;

	//Specific Sets
	std::map<gunModels, const sf::Texture* > weaponTextures;
};
