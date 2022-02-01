#ifndef MPE_LOGGER_HPP
#define MPE_LOGGER_HPP

#include <string>
#include <bitset>

namespace mpe
{
	//Severity of a log message
	enum class LogSeverity : unsigned char
	{
		Info = 0,
		Warning,
		Error,
		Count
	};

	//Source of a log message
	enum class LogOrigin : unsigned char
	{
		RenderingApi = 0,
		MPE,
		User,
		Count
	};

	//Output target of a log message
	enum class LogTarget : unsigned char
	{
		StdOut = 0,
		ExternalBuffer,
		File,
		Count
	};

	//Allows logging on standard output, file or string buffer
	class Logger
	{
	public:
		//Initializes for logging on stdout
		Logger();
		//Flushes on destruction
		~Logger();
		//Adds a message to the buffer
		void Log(const std::string& message, LogSeverity severity = LogSeverity::Info, LogOrigin origin = LogOrigin::User);
		//Outputs the buffer depending on the included targets and calls Reset()
		void Flush();
		//Resets the buffer without flushing
		void Reset();
		//Includes logs of the specified severity
		void IncludeLogs(LogSeverity severity);
		//Excludes logs of the specified severity
		void ExcludeLogs(LogSeverity severity);
		//Enables output on the specified target
		void IncludeTarget(LogTarget target);
		//Disables output on the specified target
		void ExcludeTarget(LogTarget target);
		//Flushing will write the output to the specified file. No need to call IncludeTarget
		void WriteToFile(const std::string& filename);
		//Flushing will append the output to the specified string. No need to call IncludeTarget
		void WriteToBuffer(std::string& some_buffer);
		//Returns true if the Logger is writing to the specified target
		bool IsWritingTo(LogTarget target) const;
		//Returns true if the Logger is filtering out messages of the specified severity
		bool IsIgnoring(LogSeverity severity) const;
		//Initial buffer size. Actual buffer size might change if the Logger receives large messages
		static constexpr size_t INITIAL_BUFFER_SIZE = 512;
	private:
		std::string BuildLog(const std::string& message, LogSeverity severity, LogOrigin origin) const;
		bool CanFit(const std::string& log) const;
		std::bitset<static_cast<size_t>(LogSeverity::Count)> filter;
		std::bitset<static_cast<size_t>(LogTarget::Count)> targets;
		std::string* externalBuffer;
		std::string output;
		std::string filename;
	};
}
#endif