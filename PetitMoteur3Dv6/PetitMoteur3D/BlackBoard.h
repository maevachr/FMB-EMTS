#pragma once

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
		float chrono = 180.f;
		const float MAX_BOOST = 100.0f;
		float boost = MAX_BOOST;
		float USE_MIN_WHEN_UP = 10.0f;
		bool usedBoost = false;
		bool boostDown = true;

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
		void Update(float TempsEcoule) {
			chrono -= TempsEcoule;
			boost += TempsEcoule * 4.0f;
			if (boost > MAX_BOOST) {
				boost = MAX_BOOST;
			}
			if (usedBoost)
			{
				boost -= TempsEcoule * 40.0f;
				usedBoost = false;
			}	
			if (boost > USE_MIN_WHEN_UP)
			{
				boostDown = true;
			}
		}
		float GetChrono() const { return chrono; }
		float GetBoost() const { return boost; }
	};
}
