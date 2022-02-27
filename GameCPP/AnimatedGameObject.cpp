

#include "AnimatedGameObject.h"

AnimatedGameObject::AnimatedGameObject(float spriteInterval, PlayerType playerType) {
	m_spriteInterval = spriteInterval;
	m_spriteIntervalMove = spriteInterval;
	m_playerType = playerType;
	if (m_playerType == local || m_playerType == remote || m_playerType == ai) {
		m_spriteIntervalIdle = m_spriteIntervalMove + 1.85f;
		m_healthBarOutline = new sf::RectangleShape(sf::Vector2<float>(300, 25));
		m_healthBarOutline->setFillColor(sf::Color::Transparent);
		m_healthBarOutline->setOutlineColor(sf::Color::Yellow);
		m_healthBarOutline->setOutlineThickness(3.5f);
		m_healthBarFill = new sf::RectangleShape(sf::Vector2<float>(300, 25));
		m_healthBarFill->setFillColor(sf::Color::Green);
		if (m_playerType == local) {
			m_healthBarOutline->setPosition(40, 30);
			m_healthBarFill->setPosition(40, 30);
		}
		else {
			m_healthBarOutline->setPosition(440, 30);
			m_healthBarFill->setPosition(440, 30);
		}
	}
	m_currentTextureRect = 0;
}

AnimatedGameObject::~AnimatedGameObject() {
	if (m_playerType == local || m_playerType == remote || m_playerType == ai) {
		delete m_healthBarOutline;
		delete m_healthBarFill;
	}
}

bool AnimatedGameObject::load(const std::string& filename) {
	if (m_texture.loadFromFile(filename)) {
		m_texture.setSmooth(true);
		m_sprite.setTexture(m_texture);
		m_filename = filename;
		m_valid = true;
		return true;
	}
	return false;
}


void AnimatedGameObject::draw(sf::RenderWindow& window) {
	if (m_valid) {
		window.draw(m_sprite);
		if (m_playerType == local || m_playerType == remote || m_playerType == ai) {
			m_healthBarFill->setSize(sf::Vector2<float>(m_health * 3, 25));
			window.draw(*m_healthBarOutline);
			window.draw(*m_healthBarFill);
		}
	}
}

void AnimatedGameObject::update() {
	if (m_clock.getElapsedTime().asSeconds() > m_spriteInterval) {
		nextTextureRect();
		m_clock.restart();
		if (m_animationTriggered) {
			++m_counter;
		}
	}
	if (m_counter == m_intRectRef->size()) {
		m_counter = 0;
		m_animationTriggered = false;
		useAnimations(none);
	}
	switch (m_playerType) {
	case strobe: {
		sf::Vector2<float> currentPos = this->getPosition();
		if (m_strobeDestination - m_strobeInitial > 0
			&& m_strobeDestination - currentPos.x > 0)
			move(sf::Vector2<float>(40.0f, 0) * m_animationClock.getElapsedTime().asSeconds());
		else if (m_strobeDestination - m_strobeInitial < 0
			&& m_strobeDestination - currentPos.x < 0)
			move(sf::Vector2<float>(-40.0f, 0) * m_animationClock.getElapsedTime().asSeconds());
		else
			setStrobeInterval(m_strobeInitial - m_strobeDestination);
		m_animationClock.restart();
	}
	};
}

void AnimatedGameObject::move(sf::Vector2f loc) {
	if (m_valid)
		m_sprite.move(loc);
}

void AnimatedGameObject::setPosition(float x, float y) {
	if (m_valid)
		m_sprite.setPosition(x, y);
}

sf::Vector2f AnimatedGameObject::getPosition() const {
	if (m_valid)
		return m_sprite.getPosition();
	else
		return sf::Vector2f(0, 0);
}


void AnimatedGameObject::setScale(float scale) {
	if (m_valid)
		m_sprite.setScale(scale, scale);
}

void AnimatedGameObject::addTextureRect(const sf::Rect<int> rectangle) {
	m_intRectRef->push_back(rectangle);
	this->setTextureRect(m_intRectRef->size() - 1);
}

void AnimatedGameObject::nextTextureRect() {
	if (m_currentTextureRect < (int)m_intRectRef->size() - 1 && !m_intRectRef->empty()) {
		std::vector<sf::Rect<int>>& rectVec = *m_intRectRef;
		sf::Rect<int>& intRect = rectVec[++m_currentTextureRect];
		m_sprite.setTextureRect(intRect);
	}
	else if (!m_intRectRef->empty()) {
		m_currentTextureRect = 0;
		std::vector<sf::Rect<int>>& rectVec = *m_intRectRef;
		sf::Rect<int>& intRect = rectVec[m_currentTextureRect];
		m_sprite.setTextureRect(intRect);
	}
	else {
		throw "TextureRect vector is empty";
	}
}

void AnimatedGameObject::setTextureRect(int ind) {
	std::vector<sf::Rect<int>>& rectVec = *m_intRectRef;
	if (ind < (int)m_intRectRef->size() && !rectVec.empty()) {
		sf::Rect<int>& intRect = rectVec[ind];
		m_sprite.setTextureRect(intRect);
	}
}


void AnimatedGameObject::setSpriteInterval(float spriteInterval) { m_spriteInterval = spriteInterval; }

void AnimatedGameObject::setStrobeInterval(float strobeInterval) {
	m_strobe = true;
	m_strobeInitial = this->getPosition().x;
	m_strobeDestination = m_strobeInitial + strobeInterval;
}

void AnimatedGameObject::useAnimations(Direction dir) {
	switch (dir) {
	case up: {
		m_intRectRef = &m_intRectsUp;
		this->setSpriteInterval(m_spriteIntervalMove);
		if (!m_isStuck)
			m_lastDirection = up;
		break;
	}
	case down: {
		m_intRectRef = &m_intRectsDown;
		this->setSpriteInterval(m_spriteIntervalMove);
		if (!m_isStuck)
			m_lastDirection = down;
		break;
	}
	case left: {
		m_intRectRef = &m_intRectsLeft;
		this->setSpriteInterval(m_spriteIntervalMove);
		if (!m_isStuck)
			m_lastDirection = left;
		break;
	}
	case right: {
		m_intRectRef = &m_intRectsRight;
		this->setSpriteInterval(m_spriteIntervalMove);
		if (!m_isStuck)
			m_lastDirection = right;
		break;
	}
	case punch: {
		m_intRectRef = &m_intRectsPunch;
		this->setSpriteInterval(m_spriteIntervalMove);
		break;
	}
	case crouchPunch: {
		m_intRectRef = &m_intRectsCrouchPunch;
		this->setSpriteInterval(m_spriteIntervalMove);
		break;
	}
	case block: {
		m_intRectRef = &m_intRectsBlock;
		this->setSpriteInterval(m_spriteIntervalMove);
		break;
	}
	case crouchBlock: {
		m_intRectRef = &m_intRectsCrouchBlock;
		this->setSpriteInterval(m_spriteIntervalMove);
		break;
	}
	case kick: {
		m_intRectRef = &m_intRectsKick;
		this->setSpriteInterval(m_spriteIntervalMove);
		break;
	}
	case crouchKick: {
		m_intRectRef = &m_intRectsCrouchKick;
		this->setSpriteInterval(m_spriteIntervalMove);
		break;
	}
	case none: {
		m_intRectRef = &m_intRectsNone;
		if (m_playerType == local || m_playerType == remote || m_playerType == ai)
			this->setSpriteInterval(m_spriteIntervalIdle);
		else
			this->setSpriteInterval(m_spriteIntervalMove);

		if (!m_isStuck)
			m_lastDirection = none;
		break;
	}
	};
}

float AnimatedGameObject::getSpeed() { return m_speed; }

GameObject::Direction AnimatedGameObject::getLastDirection() { return m_lastDirection; }

GameObject::PlayerType AnimatedGameObject::getPlayerType() { return m_playerType; }

AnimatedGameObject::AnimatedGameObject() {}
