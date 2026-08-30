#include "GalleryHelp.hpp"

bool GalleryHelpPopup::init()
{
	if (!Popup::init(410.f, 240.f, "GJ_square02.png"))
		return false;

	//	Background
	auto background = NineSlice::create("square02b_001.png", {0, 0, 80, 80});
	background->setContentSize({200, 340});
	background->setAnchorPoint({0, 0.5});
	background->setColor({0, 0, 0});
	background->setOpacity(50);
	background->setScale(0.5);
	background->setID("background");
	m_mainLayer->addChildAtPosition(background, Anchor::Left, ccp(10, 5), false);

	//	Title label (I think there's already one in here)
	auto label = CCLabelBMFont::create("Icon Gallery - Help", "goldFont.fnt");
	label->setAnchorPoint({0.5, 0});
	label->setScale(0.75);
	label->setID("title");
	m_mainLayer->addChildAtPosition(label, Anchor::Top, ccp(0, -25), false);

	//	The menu for the tab selection
	m_selectMenu = CCMenu::create();
	m_selectMenu->setContentSize({100, 170});
	m_selectMenu->setAnchorPoint({0, 0.5});
	m_selectMenu->setLayout(ColumnLayout::create()
								->setCrossAxisLineAlignment(AxisAlignment::Start)
								->setCrossAxisOverflow(false)
								->setAutoScale(false)
								->setAxisReverse(true)
								->setGap(12));
	m_mainLayer->addChildAtPosition(m_selectMenu, Anchor::Left, ccp(10, 5), false);

	//	The Categories
	std::vector<gd::string> text = {
		"About", "How to Use", "Downloading", "Uploading", "FAQ", "Support"};

	//	Creates the button for the categories
	for (int ii = 0; ii < text.size(); ii++)
	{
		auto buttonSpr = CCLabelBMFont::create(text[ii].c_str(), "bigFont.fnt", 100);
		buttonSpr->limitLabelWidth(100, 0.4, 0.3);
		buttonSpr->setAnchorPoint({0, 0.5});
		buttonSpr->setID("label");

		if (ii == 0)
			buttonSpr->setColor({255, 255, 0});

		auto button = CCMenuItemSpriteExtra::create(
			buttonSpr,
			this,
			menu_selector(GalleryHelpPopup::onPage));

		button->m_scaleMultiplier = 1.1f;
		button->setTag(ii);

		m_selectMenu->addChild(button);
		m_selectMenu->updateLayout();
	}

	//	Text area (for the information)
	m_textArea = MDTextArea::create("[Insert Text Here]", {280, 200});
	m_textArea->setAnchorPoint({0.5, 0.5});
	m_textArea->setID("text-area");
	m_mainLayer->addChildAtPosition(m_textArea, Anchor::Center, ccp(52.5, -10), false);

	//	Language Button Sprites
	auto offSpr = CCLabelBMFont::create("Language: ENG", "goldFont.fnt");
	offSpr->setScale(0.5f);
	auto onSpr = CCLabelBMFont::create("Language: ESP", "goldFont.fnt");
	onSpr->setScale(0.5f);

	//	Language Button
	auto m_languageButton = CCMenuItemToggler::create(
		offSpr,
		onSpr,
		this,
		menu_selector(GalleryHelpPopup::onLanguage));
	m_languageButton->m_offButton->m_scaleMultiplier = 1.1f;
	m_languageButton->m_onButton->m_scaleMultiplier = 1.1f;
	m_languageButton->setID("language-button");
	m_languageButton->toggle(m_spanish);
	m_buttonMenu->addChildAtPosition(m_languageButton, Anchor::BottomLeft, ccp(60, 25), false);

	//	Calls the function to set the info
	setInfo();

	this->setID("gallery-help-popup");
	return true;
}

void GalleryHelpPopup::onLanguage(CCObject *sender)
{
	auto button = static_cast<CCMenuItemToggler *>(sender);
	button->toggle(m_spanish);
	m_spanish = !m_spanish;

	//	Reloads the info
	setInfo();
}

void GalleryHelpPopup::onPage(CCObject *sender)
{
	auto tag = sender->getTag();

	if (m_page == tag)
		return;

	auto prevPage = m_page;
	m_page = tag;

	//	The old tab label gets colored back
	if (auto oldBtn = static_cast<CCMenuItemSpriteExtra *>(m_selectMenu->getChildByTag(prevPage)))
	{
		auto text = static_cast<CCLabelBMFont *>(oldBtn->getChildByID("label"));
		text->setColor({255, 255, 255});
	}

	//	The new tab label gets the color
	if (auto btn = static_cast<CCMenuItemSpriteExtra *>(m_selectMenu->getChildByTag(m_page)))
	{
		auto text = static_cast<CCLabelBMFont *>(btn->getChildByID("label"));
		text->setColor({255, 255, 0});
	}

	setInfo();
}

void GalleryHelpPopup::setInfo()
{
	if (!m_textArea)
		return;

	gd::string info;

	//	This is terrible, I know, but it works.
	switch (m_page)
	{
	case 0:
		//	ABOUT
		info = fmt::format(
			"{}{}{}",
			"# About This",
			"\nThe <co>**Icon Gallery**</c> started as a website made to act as a museum of public <cp>custom icons</c>, so people could download them and add them in their games.",
			"\n\n<cc>*Please be aware, that the servers of this mod might not be stable enough to handle many requests at once, so if Icons don't load properly in the gallery, this is the reason*</c>");

		if (m_spanish)
		{
			info = fmt::format(
				"{}{}{}",
				"# Acerca de",
				"\nLa <co>**Icon Gallery**</c> empezo como una pagina web que actua como un museo de <cp>iconos custom</c> publicos, para que la gente pueda descargarlos y agregarlos en sus juegos.",
				"\n\n<cc>*Ten en cuenta que es posible que los servidores de este mod no sean lo suficientemente estables para manejar muchas solicitudes al mismo tiempo, asi que si los iconos no se cargan correctamente en la galleria, esa es la razon.*</c>");
		}
		break;

	case 1:
		//	HOW TO USE
		info = fmt::format(
			"{}{}{}{}{}{}",
			"# How to Use",
			"\nThe mod will automatically set up a <cc>Texture Pack</c> upon start-up, where all the icons downloaded from the gallery will be saved by default.",
			"\n\nThe mod does **NOT** automatically apply the texture pack, so you have to do that yourself inside the <cg>Texture Loader</c> manager.",
			"\n***\n### Other Locations:",
			"\nIn the settings, you can enable special locations to download icons if you don't want a texture pack, such as the More Icons folder (for general) or the original icons (for vanilla).",
			"\n\n<cr>Be advised, that you could override icons that are already there</c>.");

		if (m_spanish)
		{
			info = fmt::format(
				"{}{}{}{}{}{}",
				"# Como Usar",
				"\nEl mod te crea automaticamente un <cc>Texture Pack</c> al inicio, ahi es donde todos los iconos descargados de la galeria seran guardados por default.",
				"\n\nEl mod **NO** aplica automaticamente el Texture Pack, tienes que hacerlo tu mismo dentro del manager de <cg>Texture Loader</c>.",
				"\n***\n### Otros Lugares:",
				"\nEn la configuracion, tu puedes activar otros lugares donde los iconos descargados terminaran si no quieres un texture pack, como la carpeta de More Icons (en general) o la carpeta de los iconos del juego (para los de formato vanilla).",
				"\n\n<cr>Ten en cuenta, que podrias reemplazar iconos que ya esten ahi</c>.");
		}
		break;

	case 2:
		//	DOWNLOADING
		info = fmt::format(
			"{}{}{}{}{}",
			"# Downloading Icons",
			"\nOnce you find the icons you wish to download, simply click the \"<co>Download</c>\" button at the right side of the icon.",
			"\n***\n### Formats:",
			"\n* **Icons with the \"<cf>Vanilla</c>\" format** are found inside the \"Icons\" folder of the Texture Pack.",
			"\n* **Icons with the \"<cp>More Icons</c>\" format** are found inside their gamemode-specific folder of <cl>More Icons</c>.");

		if (m_spanish)
		{
			info = fmt::format(
				"{}{}{}{}{}",
				"# Descargar Iconos",
				"\nSi encuentras un icono que deseas descargar, presiona el boton de <co>Descarga</c> a la derecha del icono.",
				"\n***\n### Formatos:",
				"\n* **Iconos con el formato \"<cf>Vanilla</c>\"** se encuentran en la carpeta de \"Icons\" del texture pack.",
				"\n* **Iconos con el formato \"<cp>More Icons</c>\"** se encuentran en su carpeta especifica del Gamemode de <cl>More Icons</c>");
		}
		break;

	case 3:
		//	UUPLOADING
		info = fmt::format(
			"{}{}{}",
			"# Uploading Icons",
			"If you wish to upload your own icons, visit the <cg>Website version of the Gallery</c> go to the submitting page (via the bottom right button at the website) and follow the steps to submission.",
			"\n\n<cc>*Please* ***read the guidelines*** *and make sure your icon follows the right format. Any icon that breaks either will have higher chances of rejection.*</c>");

		if (m_spanish)
		{
			info = fmt::format(
				"{}{}{}",
				"# Publicar Iconos",
				"\nSi deseas publicar tu propio icono aqui, visita la <cg>Pagina web de la Icon Gallery</c>, ve al boton inferior derecho de la misma para ir a la pagina de subir icono y sigues los pasos en como subir tus iconos.",
				"\n\n<cc>*Por favor,* ***lee las Guidelines*** *de la galeria y asegurate que tu icono sigue el formato correcto. Cualquier icono que rompe ambas cosas tiene mayores riesgos de rechazo.*</c>");
		}
		break;

	case 4:
		//	FAQ
		info = fmt::format(
			"{}{}{}{}{}{}{}{}{}{}",
			"# FAQ",
			"\nThis is a list of frequently asked questions, just in case you need to look for more help about the mod, join the discord server.",
			"\n***\n### How do I equip the icons?",
			"\nIf you downloaded icons via the mod, you should already have a Icon pack folder assigned it, go to the <cg>Texture Loader</c> and move the pack named \"<co>Downloaded Icons</c>\" into the right column and refresh the textures.",
			"\n***\n### I can't find an Icon I downloaded",
			"\nReload your textures if you don't see any changes. Some icons might have the \"<cf>Vanilla</c>\" format, thus they take the place of existing vanilla designs.",
			"\n***\n### I want to report an Icon",
			"\nIf you came across with an icon that doesn't load properly, or has incorrect author credits, make sure to report it to the admins of the Gallery via the <cd>discord server</c>.",
			"\n***\n### I'm having connection issues",
			"\nIf you're having connection issues with the mod, please be patient. This can either mean that you have internet connection issues, rate limitations or it's a server-side issue.");

		if (m_spanish)
		{
			info = fmt::format(
				"{}{}{}{}{}{}{}{}{}{}",
				"# FAQ",
				"\nEsta es una lista de preguntas frecuentes, se ira actualizando al traves del tiempo. Si necesitas mas ayuda sobre el mod, unite al servidor de Discord.",
				"\n***\n### Como me aplico los iconos?",
				"\nSi los iconos si descargan por medio del mod, ya debes tener una carpeta destino de los mismos. Ve al <cg>Texture Loader</c> y mueve el Pack llamado \"<co>Downloaded Icons</c>\" (Iconos descargados) a la columna derecha del loader y recarga las texturas del juego.",
				"\n***\n ### No encuentro un icono que descargue",
				"\nRecarga tus texturas si no vez algun cambio. Algunos iconos pueden ser de formato \"<cf>Vanilla</c>\", y por ende, toman el lugar de un icono existente del juego.",
				"\n***\n ### Quiero reportar un Icono",
				"\nSi te haz encontrado un icono que no carga, o cuyos creditos no son los correctos, reportalo a los admins de la Galeria en el <cd>Servidor de Discord</c>.",
				"\n***\n ### Tengo problemas de conexion",
				"\nSi tienes problemas de conexion con el mod, ten paciencia. Es posible que tengas problemas de internet, limitada la conexion o es un problema del servidor de la Galeria.");
		}
		break;

	case 5:
		//	SUPPORT
		info = fmt::format(
			"{}{}",
			"# Support",
			"\nIf you come across bugs with the mod itself or wish to support the development of the gallery in any way, make sure to contact us through the [GitHub repository](https://github.com/MasterL500/IconGallery-geode) or through [Asterveila's Discord Server](https://discord.gg/dceY3uvGzD).");

		if (m_spanish)
		{
			info = fmt::format(
				"{}{}",
				"# Soporte",
				"\nSi encuentras un bug del mod mismo o deseas ayudar el desarrollo de la galeria (ya sea el Mod o la pagina web) de alguna manera, asegurate de contactarnos a travs del [Repositorio Github](https://github.com/MasterL500/IconGallery-geode) o por medio del [Servidor de Discord](https://discord.gg/dceY3uvGzD).");
		}
		break;
	}

	m_textArea->setString(info.c_str());
};

GalleryHelpPopup *GalleryHelpPopup::create()
{
	auto ret = new GalleryHelpPopup;

	if (ret->init())
	{
		ret->autorelease();
		return ret;
	};

	delete ret;
	return nullptr;
}