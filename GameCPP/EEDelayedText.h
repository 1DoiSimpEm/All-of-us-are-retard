#include <SFML\Graphics.hpp>
class EEDelayedText : public sf::Text
{
private:
    sf::Clock m_Clock;
    sf::Time m_time, m_delay;
    std::string m_string, m_buffer;
    unsigned int current;
public:
    EEDelayedText(const std::string& gstring, const sf::Font& gfont, unsigned int gsize);
    ~EEDelayedText(void);
    void setString(const std::string& string);//covering text's setter
    void setDelay(sf::Time gtime);
    const EEDelayedText& update(void);
};