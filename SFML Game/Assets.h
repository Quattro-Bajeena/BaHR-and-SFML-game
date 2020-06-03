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
	std::map<std::string, sf::Music> music;

	std::map<std::string, sf::SoundBuffer> soundBuffers;
	std::map<std::string, sf::Sound> sounds;

	//Specific Sets
	std::map<gunModels, const sf::Texture* > weaponTextures;
};
