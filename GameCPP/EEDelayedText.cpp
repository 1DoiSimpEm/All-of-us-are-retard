#include "EEDelayedText.h"
EEDelayedText::EEDelayedText(const std::string& gstring, const sf::Font& gfont, unsigned int gsize) :
    sf::Text("", gfont, gsize),//can be changes probably, my sfml 2.0 is not latest so I miss some ctors
    current(0),
    m_string(gstring)
{
}
EEDelayedText::~EEDelayedText(void)
{
}
void EEDelayedText::setString(const std::string& gstring)
{
    m_buffer.clear();
    m_string = gstring;
    current = 0;
}
void EEDelayedText::setDelay(sf::Time gtime)
{
    m_delay = gtime;
}
const EEDelayedText& EEDelayedText::update(void)
{
    m_time += m_Clock.restart();
    while (m_time >= m_delay)
    {
        m_time -= m_delay;
        if (current < m_string.length())
        {
            m_buffer += m_string[current];
            ++current;
        }
    }
    sf::Text::setString(m_buffer);//text's setString is covered, must call this way
    return *this;
}