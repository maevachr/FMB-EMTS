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

	public:
		void Update(float TempsEcoule) {
			chrono -= TempsEcoule;
		}
		float GetChrono(){ return chrono; }
	};
}
