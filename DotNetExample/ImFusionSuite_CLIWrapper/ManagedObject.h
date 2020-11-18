#pragma once

using namespace System;
namespace ImFusion
{
	namespace CLI {

		using namespace System::Runtime::InteropServices;
		static const char* string_to_char_array(String^ string)
		{
			const char* str = (const char*)(Marshal::StringToHGlobalAnsi(string)).ToPointer();
			return str;
		}

		template<class T>
		public ref class ManagedObject : public IDisposable
		{
		protected:
			T* m_instance;
		public:
			ManagedObject(T* instance)
				: m_instance(instance)
			{
			}
			virtual ~ManagedObject()
			{
				if (m_instance != nullptr)
				{
					delete m_instance;
					m_instance = nullptr;
				}
			}
			!ManagedObject()
			{
				if (m_instance != nullptr)
				{
					delete m_instance;
					m_instance = nullptr;
				}
			}
			T* get()
			{
				return m_instance;
			}

			void reset()
			{
				if (m_instance != nullptr)
				{
					delete m_instance;
					m_instance = nullptr;
				}
			}
		};
	}
}