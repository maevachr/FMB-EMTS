#pragma once
#include "SoundManager.h"

namespace PM3D
{
	class BlackBoard
	{
	private:
		BlackBoard() = default;
		~BlackBoard() = default;
	public:
		BlackBoard(const BlackBoard&) = delete;
		BlackBoard operator=(const BlackBoard&) = delete;

	public:
		static BlackBoard& GetInstance()
		{
			static BlackBoard bb;
			return bb;
		}

	private:
		int score{};
		float chrono = 235.f;		const float MAX_BOOST = 100.0f;
		float boost = MAX_BOOST;
		float USE_MIN_WHEN_UP = 10.0f;
		bool usedBoost = false;
		bool boostDown = true;
		float vitesse;
		int rank1 = 0;
		int rank2 = 0;
		int rank3 = 0;

	public:
		void AddPoints(int nbPoints) { 
			score += nbPoints;
		}
		int GetScore() { return score; }
		void AddBoost(float b) {
			boost += b;
		}
		bool UseBoost() {
			if (boost > 0 && boostDown) {
				usedBoost = true;
				return true;
			}
			else if (boost < 5.0f)
			{
				boostDown = false;
			}
			return false;
		}
		void Update(float TempsEcoule);
		float GetChrono() const { return chrono; }
		float GetBoost() const { return boost; }

		void IncreaseRank(int val) {
			if(val == 100)	++rank1; 
			if(val == 200)	++rank2; 
			if(val == 500)	++rank3; 
		}
		 
		int GetRank1() { return rank1; }
		int GetRank2() { return rank2; }
		int GetRank3() { return rank3; }
	};
}
