#include <MPE/Core/Logger.hpp>
#include <MPE/Core/Utils.hpp>


int main()
{
	mpe::Logger& logger = mpe::Global<mpe::Logger>();
	logger.Log(
		"\"main\" function should contain some test :/",
		mpe::LogSeverity::Warning,
		mpe::LogOrigin::User
	);

	return 0;
}