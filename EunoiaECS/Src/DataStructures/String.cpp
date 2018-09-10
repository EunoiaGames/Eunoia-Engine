#include "String.h"

namespace Eunoia {

	String::String(const char * chars)
	{
		Copy(chars);
	}

	String::String(const String & string)
	{
		Copy(string);
	}

	String::String(uint32 length)
	{
		m_length = length;
		m_chars = new char[m_length + 1];
		m_chars[m_length] = '\0';
	}

	String::String()
	{
		m_chars = NULL;
		m_length = 0;
	}

	String::~String()
	{
		delete[] m_chars;
	}

	uint32 String::Length() const
	{
		return m_length;
	}

	const char * String::C_Str() const
	{
		return m_chars;
	}

	String String::SubString(uint32 startIndex, uint32 endIndex) const
	{
		String substring(endIndex - startIndex);

		for (uint32 i = 0; i < substring.Length(); i++)
		{
			substring[i] = m_chars[startIndex + i];
		}

		return substring;
	}

	String String::SubString(uint32 startIndex) const
	{
		return SubString(startIndex, m_length);
	}

	uint32 String::GetRepeatCount(const String& string) const
	{
		uint32 count = 0;
		int32 index = 0;
		while ((index = FindFirstOf(string, index + 1)) != -1)
		{
			count++;
		}

		return count;
	}

	std::vector<String> String::Split(const String & delimiter) const
	{
		std::vector<String> strings;

		uint32 index = FindFirstOf(delimiter);
		strings.push_back(SubString(0, index));
		while (true)
		{
			int newIndex = FindFirstOf(delimiter, index + 1);
			if (newIndex == -1)
			{
				break;
			}

			strings.push_back(SubString(index + 1, newIndex));
			index = newIndex;
		}

		strings.push_back(SubString(index + 1));

		return strings;
	}

	bool String::IsEmpty() const
	{
		return m_length == 0;
	}

	bool String::IsWhiteSpace() const
	{
		for (uint32 i = 0; i < m_length; i++)
		{
			if (m_chars[i] != ' ' && m_chars[i] != '\t' && m_chars[i] != '\n')
			{
				return false;
			}
		}

		return true;
	}

	bool String::Contains(const String & string) const
	{
		if (string.Length() > m_length)
		{
			return false;
		}

		unsigned int counter = 0;

		for (uint32 i = 0; i < string.Length(); i++)
		{
			if (m_chars[i] == string[counter])
			{
				counter++;
			}
			else
			{
				counter = 0;
			}

			if (counter == string.Length())
			{
				return true;
			}
		}

		return false;
	}

	int32 String::FindFirstOf(const String & string, uint32 start) const
	{
		uint32 counter = 0;

		for (uint32 i = start; i < m_length; i++)
		{
			bool match = true;

			for (uint32 j = 0; j < string.Length(); j++)
			{
				if (m_chars[i + j] != string[j])
				{
					match = false;
					break;
				}
			}

			if (match)
			{
				return i;
			}
		}

		return -1;
	}

	int32 String::FindFirstNotOf(const String & string, uint32 start) const
	{
		uint32 counter = 0;

		for (uint32 i = start; i < m_length; i += string.Length())
		{
			bool match = true;

			for (uint32 j = 0; j < string.Length(); j++)
			{
				if (m_chars[i + j] != string[j])
				{
					match = false;
					break;
				}
			}

			if (!match)
			{
				return i;
			}
		}

		return -1;
	}

	int32 String::FindLastOf(const String & string) const
	{
		uint32 index = 0;
		uint32 counter = 0;

		for (uint32 i = m_length - 1; i > 0; i--)
		{
			if (m_chars[i] == string[string.Length() - counter - 1])
			{
				if (counter == 0)
				{
					index = i - string.Length() + 1;
				}

				counter++;
			}
			else
			{
				index = 0;
				counter = 0;
			}

			if (counter == string.Length())
			{
				return index;
			}
		}

		return -1;
	}

	bool String::BeginsWith(const String & string) const
	{
		if (m_length < string.Length())
		{
			return false;
		}

		for (uint32 i = 0; i < string.Length(); i++)
		{
			if (m_chars[i] != string[i])
			{
				return false;
			}
		}

		return true;
	}

	String String::Cut(uint32 start, uint32 end) const
	{
		String cut(m_length - (end - start + 1));

		for (uint32 i = 0; i < start; i++)
		{
			cut[i] = m_chars[i];
		}

		uint32 counter = start;
		for (uint32 i = end + 1; i < m_length; i++)
		{
			cut[counter++] = m_chars[i];
		}

		return cut;
	}

	String String::RemoveFirstOf(const String & string) const
	{
		int32 firstOf = FindFirstOf(string);

		if (firstOf == -1)
		{
			return *this;
		}

		return this->Cut(firstOf, firstOf + string.Length() - 1);
	}

	String String::RemoveLastOf(const String & string) const
	{
		int32 lastOf = FindLastOf(string);

		if (lastOf == -1)
		{
			return *this;
		}

		return this->Cut(lastOf, lastOf + string.Length() - 1);
	}

	String String::RemoveAllOf(const String & string) const
	{
		String newString = *this;

		while (true)
		{
			int32 firstOf = newString.FindFirstOf(string);

			if (firstOf == -1)
			{
				return newString;
			}
			else
			{
				newString = newString.Cut(firstOf, firstOf + string.Length() - 1);
			}
		}
	}

	String String::TrimBeginning() const
	{
		for (uint32 i = 0; i < m_length; i++)
		{
			if (m_chars[i] != ' ' && m_chars[i] != '\t' && m_chars[i] != '\n')
			{
				return SubString(i);
			}
		}

		return *this;
	}

	String String::TrimEnd() const
	{
		for (uint32 i = m_length - 1; i > 0; i--)
		{
			if (m_chars[i] != ' ' && m_chars[i] != '\t' && m_chars[i] != '\n')
			{
				return SubString(0, i + 1);
			}
		}

		return *this;
	}

	String& String::operator=(const char * chars)
	{
		Copy(chars);
		return *this;
	}

	String& String::operator=(const String & string)
	{
		Copy(string);
		return *this;
	}

	bool String::operator==(const String & string) const
	{
		if (m_length != string.Length())
		{
			return false;
		}

		for (uint32 i = 0; i < m_length; i++)
		{
			if (m_chars[i] != string[i])
			{
				return false;
			}
		}

		return true;
	}

	bool String::operator!=(const String & string) const
	{
		return !(operator==(string));
	}

	bool String::operator<(const String & string) const
	{
		return std::lexicographical_compare(m_chars, m_chars + m_length, string.m_chars, string.m_chars + string.m_length);
	}

	String String::operator+(const String & string) const
	{
		String newstring(m_length + string.Length());

		for (uint32 i = 0; i < m_length; i++)
		{
			newstring[i] = m_chars[i];
		}

		for (uint32 i = 0; i < string.Length(); i++)
		{
			newstring[m_length + i] = string[i];
		}

		return newstring;
	}

	String& String::operator+=(const String & string)
	{
		char* chars = new char[m_length + string.Length() + 1];

		for (uint32 i = 0; i < m_length; i++)
		{
			chars[i] = m_chars[i];
		}

		for (uint32 i = 0; i < string.Length(); i++)
		{
			chars[m_length + i] = string[i];
		}

		chars[m_length + string.Length()] = '\0';

		delete m_chars;
		m_chars = chars;
		m_length = m_length + string.Length();

		return *this;
	}

	char String::operator[](uint32 index) const
	{
		return m_chars[index];
	}

	char & String::operator[](uint32 index)
	{
		return m_chars[index];
	}

	void String::Copy(const char * chars)
	{
		m_length = 0;

		while (chars[m_length] != '\0')
		{
			m_length++;
		}

		m_chars = new char[m_length + 1];

		for (uint32 i = 0; i < m_length; i++)
		{
			m_chars[i] = chars[i];
		}

		m_chars[m_length] = '\0';
	}

	void String::Copy(const String & string)
	{
		m_length = string.Length();
		m_chars = new char[m_length + 1];
		for (uint32 i = 0; i < m_length; i++)
		{
			m_chars[i] = string[i];
		}

		m_chars[m_length] = '\0';
	}

	std::ostream & operator<<(std::ostream & stream, const String & string)
	{
		stream << string.C_Str();
		return stream;
	}

}
