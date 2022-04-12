#include <dxgi1_4.h>
#include <d3d12.h>
#include <stdio.h>

void Error(const char *str)
{
    fprintf(stderr, "ERROR: %s\n", str);
    Sleep(INFINITE);
}

void GetHardwareAdapter(IDXGIFactory4* pFactory, IDXGIAdapter1** ppAdapter)
{
    *ppAdapter = nullptr;
    for (UINT AdapterIndex = 0; ; ++AdapterIndex)
    {
        IDXGIAdapter1* pAdapter = nullptr;
        if (DXGI_ERROR_NOT_FOUND == pFactory->EnumAdapters1(AdapterIndex, &pAdapter;))
        {
            break;
        }

        if (SUCCEEDED(D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
        {
            *ppAdapter = pAdapter;
            return;
        }
        pAdapter->Release();
    }
}

int main(int argc, char *argv[])
{
    IDXGIFactory4* pFactory = nullptr;
    if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&pFactory;))))
    {
        Error("CreateDXGIFactory1 failed");
    }

    IDXGIAdapter1* pAdapter = nullptr;
    GetHardwareAdapter(pFactory, &pAdapter;);
    if (!pAdapter)
    {
        Error("Failed to find DX12-compatible DXGI adapter");
    }

    ID3D12Device* pDevice = nullptr;
    if (FAILED(D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&pDevice;))))
    {
        Error("D3D12CreateDevice failed for Adapter");
    }

    if (FAILED(pDevice->SetStablePowerState(TRUE)))
    {
        Error("SetStablePowerState failed. Do you have the Win10 SDK installed?");
    }

    printf("SUCCESS. Close this program to restore default clocks.\n");
    Sleep(INFINITE);

    return 0;
}
