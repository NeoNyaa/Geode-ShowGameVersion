#include <unordered_map>
#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

struct labelPosition {
	float posX, posY;
	float anchorX, anchorY;
	CCTextAlignment align;
};

class $modify(MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) return false;

		// Config
		bool confGDVersion = Mod::get()->getSettingValue<bool>("conf-showGDVersion");
		bool confGeodeVersion = Mod::get()->getSettingValue<bool>("conf-showGeodeVersion");
		bool confCocosVersion = Mod::get()->getSettingValue<bool>("conf-showCocos2dVersion");
		bool confShowVersionsOnNewLine = Mod::get()->getSettingValue<bool>("conf-versionsOnNewLines");
		std::string confVersionsPosition = Mod::get()->getSettingValue<std::string>("conf-versionsPosition");
		double confVersionAlpha = Mod::get()->getSettingValue<double>("conf-versionsOpacity");
		bool confDebugEnabled = Mod::get()->getSettingValue<bool>("conf-debugLoggingEnabled");

		// Vars
		float menuHeight = getContentHeight() - 2;
		float menuWidth = getContentWidth() - 2;
		std::string splitCharacter = (confShowVersionsOnNewLine) ? "\n" : " | ";
		std::string versionStrings[3];
		std::string labelString;

		// Get versions
		if (confGDVersion) {
			versionStrings[0] = std::string("GD: v") + Loader::get()->getGameVersion();
		}
		if (confGeodeVersion) {
			versionStrings[1] = std::string("Geode: v") + Loader::get()->getVersion().toNonVString();
		}
		if (confCocosVersion) {
			versionStrings[2] = std::string("Cocos2d: v") + cocos2d::cocos2dVersion();
		}

		// Concatenate into single string with seperators between enabled valued
		for (auto &verString : versionStrings) {
			if (!verString.empty()) {
				if (!labelString.empty()) {
					labelString += splitCharacter;
				}
				labelString += verString;
			}
		}

		// Map of lable positions
		std::unordered_map<std::string, labelPosition> layouts = {
				{"Top Left", {2, menuHeight, 0, 1, kCCTextAlignmentLeft}},
				{"Top Right", {menuWidth, menuHeight, 1, 1, kCCTextAlignmentRight}},
				{"Bottom Left", {2, 2, 0, 0, kCCTextAlignmentLeft}},
				{"Bottom Right", {menuWidth, 2, 1, 0, kCCTextAlignmentRight}},
		};

		// Make, position, align, style, then add the label
		auto labelItem = CCLabelBMFont::create(labelString.c_str(), "bigFont.fnt");
		auto labelOpts = layouts[confVersionsPosition];
		labelItem->setPosition(labelOpts.posX, labelOpts.posY);
		labelItem->setAnchorPoint({labelOpts.anchorX, labelOpts.anchorY});
		labelItem->setAlignment(labelOpts.align);
		labelItem->setScale(0.25f);
		labelItem->setOpacity(confVersionAlpha);
		labelItem->setID("client-versions-label");
		this->addChild(labelItem);

		// Print values to console
		if (confDebugEnabled) {
			log::info("GD Version: {}", versionStrings[0]);
			log::info("Geode Version: {}", versionStrings[1]);
			log::info("Cocos Version: {}", versionStrings[2]);
			log::info("Joined Versions String: {}", labelString);
		}

		return true;
	}
};
