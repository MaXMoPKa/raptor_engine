#include "gapi/inc/device.hpp"

namespace raptor_engine
{
namespace gapi
{
void Device::enableDebugLayer()
{
	ComPtr<ID3D12Debug> debugInterface;
	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface)));
	debugInterface->EnableDebugLayer();
}
} // namespace gapi
} // namespace raptor_engine