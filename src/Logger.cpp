#include <MPE/Core/Logger.hpp>
#include <chrono>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

namespace mpe
{
	Logger::Logger() :
		filter(~0),
		externalBuffer(nullptr)
	{
		output.reserve(INITIAL_BUFFER_SIZE);
		IncludeTarget(LogTarget::StdOut);
	}

	std::string severity_string(LogSeverity severity);
	std::string origin_string(LogOrigin origin);
	
	bool Logger::CanFit(const std::string& log) const
	{
		return output.capacity() - output.size() > log.size();
	}

	void Logger::IncludeLogs(LogSeverity severity)
	{
		filter.set(static_cast<size_t>(severity));
	}

	void Logger::ExcludeLogs(LogSeverity severity)
	{
		filter.reset(static_cast<size_t>(severity));
	}

	void Logger::IncludeTarget(LogTarget target)
	{
		targets.set(static_cast<size_t>(target));
	}

	void Logger::ExcludeTarget(LogTarget target)
	{
		targets.reset(static_cast<size_t>(target));
	}

	void Logger::WriteToFile(const std::string& filename)
	{
		this->filename = filename;
		IncludeTarget(LogTarget::File);
	}

	void Logger::WriteToBuffer(std::string& some_buffer)
	{
		externalBuffer = &some_buffer;
		IncludeTarget(LogTarget::ExternalBuffer);
	}

	bool Logger::IsWritingTo(LogTarget target) const
	{
		return targets.test(static_cast<size_t>(target));
	}

	bool Logger::IsIgnoring(LogSeverity severity) const
	{
		return !filter.test(static_cast<size_t>(severity));
	}

	Logger::~Logger()
	{
		Log("Logger shutdown...", LogSeverity::Info, LogOrigin::MPE);
		Flush();
	}

	void Logger::Log(const std::string& message, LogSeverity severity, LogOrigin origin)
	{
		if (IsIgnoring(severity))
			return;

		std::string log = BuildLog(message, severity, origin);

		if (!CanFit(log))
			Flush();

		output += log;
	}

	void Logger::Reset()
	{
		output.clear();
	}

	void Logger::Flush()
	{
		if (output.empty())
			return;

		if (IsWritingTo(LogTarget::StdOut))
			std::cout << output;

		if (IsWritingTo(LogTarget::File))
		{
			std::ofstream file(filename, std::ios::app);
			if (file.is_open())
				file << output;
			else
				std::cerr << "Failed to open file for logging:\t\"" << filename << '\n';
		}

		if (IsWritingTo(LogTarget::ExternalBuffer) && (externalBuffer != nullptr))
			externalBuffer->append(output);

		Reset();
	}

	std::string Logger::BuildLog(const std::string& message, LogSeverity severity, LogOrigin origin) const
	{
		std::string log;
		log.reserve(message.size() + 64);
		
		time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		//std::string timestamp = std::string("[") + std::ctime(&now) + ']';

		std::stringstream timestamp;
		timestamp << std::put_time(std::localtime(&now), "\t[%c]\t");

		log
			.append(severity_string(severity))	//[SEVERITY]
			.append({ '\t' })
			.append(origin_string(origin))		//[SEVERITY]	[ORIGIN]
			//.append({ '\t' })
			.append(timestamp.str())			//[SEVERITY]	[ORIGIN]	[TIMESTAMP]
			//.append({ '\t' })
			.append(message)					//[SEVERITY]	[ORIGIN]	[TIMESTAMP]	MESSAGE
			.append("\n\n");

		return log;
	}

	std::string severity_string(LogSeverity severity)
	{
		std::string output;
		switch (severity)
		{
		case LogSeverity::Info:
			output = "[INFO]";
			break;
		case LogSeverity::Warning:
			output = "[WARNING]";
			break;
		case LogSeverity::Error:
			output = "[ERROR]";
			break;
		default:
			output = "[INVALID]";
			break;
		}
		return output;
	}

	std::string origin_string(LogOrigin origin)
	{
		std::string output;
		switch (origin)
		{
		case LogOrigin::RenderingApi:
			output = "[Rendering API]";
			break;
		case LogOrigin::MPE:
			output = "[MPE]";
			break;
		case LogOrigin::User:
			output = "[User]";
			break;
		default:
			output = "[INVALID]";
			break;
		}
		return output;
	}
}