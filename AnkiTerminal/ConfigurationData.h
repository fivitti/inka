#ifndef ConfigurationData_h
#define ConfigurationData_h

#define MAX_SELECTED_POSITION 255

struct ConfigurationData
{
private:
  static const byte getMaximum(const byte size)
  {
    int power = pow(10, size);
    return power < MAX_SELECTED_POSITION ? power : MAX_SELECTED_POSITION;
  }

public:
	const __FlashStringHelper * m_header;
	const byte m_numLine;
	const byte m_numField;
  const byte m_maximum;

	ConfigurationData(const __FlashStringHelper * header, const byte numLine, const byte numField, const byte fieldSize)
	                  : m_header(header), m_numLine(numLine), m_numField(numField), m_maximum(getMaximum(fieldSize)) {  }
  ConfigurationData(const __FlashStringHelper * header, const byte numLine, const byte numField, const byte fieldSize, const byte maximum)
    : m_header(header), m_numLine(numLine), m_numField(numField), m_maximum(maximum) {  }
};

#undef MAX_SELECTED_POSITION
#endif //ConfigurationData_h
