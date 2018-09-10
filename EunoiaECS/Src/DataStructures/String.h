#pragma once

#include <iostream>
#include <vector>
#include "../Core/Common.h"

namespace Eunoia {

	class String
	{
	public:
		String(const char* chars);
		String(const String& string);
		String(uint32 length);
		String();
		~String();

		unsigned int Length() const;
		const char* C_Str() const;

		String SubString(uint32 startIndex, uint32 endIndex) const;
		String SubString(uint32 startIndex = 0) const;

		uint32 GetRepeatCount(const String& string) const;

		std::vector<String> Split(const String& delimiter) const;

		bool IsEmpty() const;
		bool IsWhiteSpace() const;

		bool Contains(const String& string) const;

		int32 FindFirstOf(const String& string, uint32 start = 0) const;
		int32 FindFirstNotOf(const String& string, uint32 start = 0) const;
		int32 FindLastOf(const String& string) const;

		bool BeginsWith(const String& string) const;

		String Cut(uint32 start, uint32 end) const;

		String RemoveFirstOf(const String& string) const;
		String RemoveLastOf(const String& string) const;
		String RemoveAllOf(const String& string) const;

		String TrimBeginning() const;
		String TrimEnd() const;

		friend std::ostream& operator<<(std::ostream& stream, const String& string);

		String& operator=(const char* chars);
		String& operator=(const String& string);

		bool operator==(const String& string) const;
		bool operator!=(const String& string) const;
		bool operator<(const String& string) const;

		String operator+(const String& string) const;
		String& operator+=(const String& string);

		char operator[](uint32 index) const;
		char& operator[](uint32 index);
	private:
		void Copy(const char* chars);
		void Copy(const String& string);
		
		char* m_chars;
		uint32 m_length;
	};

}
