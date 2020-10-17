#ifndef _GEF_APPLICATION_H
#define _GEF_APPLICATION_H

#include "platform/d3d11/system/platform_d3d11.h"
namespace gef
{
	// forward declarations
	class Platform;
	class PlatformD3D11;

	class Application
	{
	public:
		Application(PlatformD3D11& platform);
		virtual ~Application();
		void Run();
		virtual void Init() = 0;
		virtual void CleanUp() = 0;
		virtual bool Update(float ticks) = 0;
		virtual void Render() = 0;
		virtual void PostFrameEnd() {};

		inline PlatformD3D11& platform() { return platform_; }

	protected:
		class PlatformD3D11& platform_;
	private:
		bool running_;
	};
}

#endif // _GEF_APPLICATION_H
