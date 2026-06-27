#ifndef DEPENDENCIES_H
#define DEPENDENCIES_H

#include "Device.h"

class Dependencies {
	private:
		static Dependencies* dependenciesInstance;
		Dependencies();

	public:
		Dependencies(const Dependencies&) = delete;
		Dependencies& operator=(const Dependencies&) = delete;
		static Dependencies* getInstance();
		Device* cimaDevice;
};

#endif