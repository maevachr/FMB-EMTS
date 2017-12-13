#pragma once
#include <fmod.hpp>

namespace PM3D
{

	using namespace FMOD;

	class SoundManager
	{
	private:
		SoundManager() = default;
		~SoundManager() = default;
	public:
		SoundManager(const SoundManager&) = delete;
		SoundManager operator=(const SoundManager&) = delete;

	public:
		static SoundManager& GetInstance()
		{
			static SoundManager sm;
			return sm;
		}

	public:
		enum SoundType {
			ENGINE, 
			EXPLODING_CRATE, 
			SCORE_BING, 
			MUSIC, 
			BOOST,
			COUNT
		};
	private:
		System		*system;
		Sound		*sound[COUNT];
		Channel		*channel[COUNT];
		
		float		engineFrequency;
		float		engineVolume;

	public:
		void Init() 
		{
			FMOD::System_Create(&system);
			system->init(32, FMOD_INIT_NORMAL, nullptr);

			system->createSound("engine.wav", FMOD_CREATESAMPLE | FMOD_LOOP_NORMAL, 0, &sound[ENGINE]);
			system->createSound("wood_explosion.mp3", FMOD_CREATESAMPLE | FMOD_LOOP_NORMAL, 0, &sound[EXPLODING_CRATE]);
			system->createSound("score.mp3", FMOD_CREATESAMPLE | FMOD_LOOP_NORMAL, 0, &sound[SCORE_BING]);
			system->createSound("dynamite.mp3", FMOD_CREATESAMPLE | FMOD_LOOP_NORMAL, 0, &sound[MUSIC]);
			system->createSound("boost.wav", FMOD_CREATESAMPLE | FMOD_LOOP_NORMAL, 0, &sound[BOOST]);

			sound[ENGINE]->setLoopCount(-1);
			sound[BOOST]->setLoopCount(-1);
			sound[EXPLODING_CRATE]->setLoopCount(0);
			sound[SCORE_BING]->setLoopCount(0);

			//Start Loops
			system->playSound(FMOD_CHANNEL_FREE, sound[BOOST], true, &channel[BOOST]);
			system->playSound(FMOD_CHANNEL_FREE, sound[ENGINE], true, &channel[ENGINE]);
			system->playSound(FMOD_CHANNEL_FREE, sound[MUSIC], true, &channel[MUSIC]);

			//Get the engine base parameter
			channel[ENGINE]->getFrequency(&engineFrequency);
			channel[ENGINE]->getVolume(&engineVolume);

			//Adapt Sound volume of music
			float musicVolume;
			channel[MUSIC]->getVolume(&musicVolume);
			channel[MUSIC]->setVolume(musicVolume * 80);
		}

		//Dont use this for music | engine | boost which are loops
		void PlaySoundEffect(SoundType id) {
			system->playSound(FMOD_CHANNEL_FREE, sound[id], false, &channel[id]);
		}
		void PauseSound(SoundType id) {
			channel[id]->setPaused(true);
		}
		void ContinueSound(SoundType id) {
			channel[id]->setPaused(false);
		}

		void Engine(float kmh) {
			channel[ENGINE]->setFrequency(engineFrequency + engineFrequency * (3 * kmh / 200));
			channel[ENGINE]->setVolume(engineVolume + engineVolume * (1.3 * kmh / 200));
		}

		void Clean() 
		{
			for (int i = 0; i < SoundType::COUNT; ++i)
			{
				sound[i]->release();
			}
			system->close();
			system->release();
		}
		
	};
}
