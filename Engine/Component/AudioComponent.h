#include "Component.h"
#include "Audio/AudioChannel.h"
#include <string>

namespace nc
{
	class AudioComponent : public Component
	{
	public:
		virtual void Update() override;

		void Play();
		void Stop();

//		<writeand read functions>
		virtual bool Write(const rapidjson::Value& value) const override;
		virtual bool Read(const rapidjson::Value& value) override;

	public:
		//<string for soundName>
		//	<volume float default to 1>
		//	<pitch float default to 1>
		//	<loop bool default to false>
		//	<playOnAwake bool default to false>
		std::string soundName;
		float volume = 1;
		float pitch = 1;
		bool loop = false;
		bool playOnAwake = false;

	private:
		AudioChannel channel;
		bool played{ false };
	};
}
