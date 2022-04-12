#include <iostream>
#include <vector>

#include <dxgi1_4.h>
#include <d3d12.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

int main() {
    ComPtr<IDXGIFactory4> factory;
    if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory)))) {
        std::wcout << L"CreateDXGIFactory1 failed!" << std::endl;
        return 1;
    }

    std::vector<ComPtr<ID3D12Device>> devices;
    ComPtr<IDXGIAdapter1> adapter;
    for (UINT i = 0; factory->EnumAdapters1(i, &adapter) != DXGI_ERROR_NOT_FOUND; ++i) {
        DXGI_ADAPTER_DESC1 desc = {};
        if (SUCCEEDED(adapter->GetDesc1(&desc))) {
            std::wcout << L"Adapter: " << desc.Description << std::endl;
        } else {
            std::wcout << L"Adapter: Unknown" << std::endl;
        }

        ComPtr<ID3D12Device> device;
        if (FAILED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device)))) {
            std::wcout << L"  D3D12CreateDevice failed!" << std::endl;
            continue;
        }

        if (FAILED(device->SetStablePowerState(TRUE))) {
            std::wcout << L"  SetStablePowerState failed! Is the Win10SDK installed and the developer mode active?" << std::endl;
            continue;
        }

        std::wcout << "  StablePowerState enabled!" << std::endl;
        devices.push_back(device);
    }

    if (devices.empty()) {
        std::wcout << L"No devices with static power state found!" << std::endl;
        return 1;
    }

    std::wcout << L"Close this program to restore default clocks." << std::endl;
    Sleep(INFINITE);

    return 0;
}
