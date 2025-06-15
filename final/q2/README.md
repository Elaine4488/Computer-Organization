# Question 2: Enable L3 Cache in GEM5 + NVMain

## 說明

本題目目標為在 GEM5 中加入 Last-Level Cache (L3)，並支援 NVMain 記憶體模組。以下為實作步驟摘要：

## 修改項目

1. **定義 L3 Cache**  
   在 `configs/common/Caches.py` 中新增 `L3Cache` 類別，設為 2-way，latency 設定為 20。

2. **建立 L2 與 L3 的連接**  
   在 `configs/common/CacheConfig.py`：
   - 新增 L3Cache 類別引用
   - 使用 `tol2bus` 和 `tol3bus` 連接 CPU ↔ L2 ↔ L3 ↔ memory
   - 調整 `if` 為 `elif` 來區分只有 L2 或同時使用 L2/L3

3. **新增 L3XBar**  
   在 `src/mem/XBar.py` 中複製 `L2XBar` 為 `L3XBar`，作為 L2 與 L3 之間的交叉匯流排。

4. **將 L3XBar 接入 CPU 架構**  
   在 `src/cpu/BaseCPU.py` 中引入 `L3XBar`，並新增 `addThreeLevelCacheHierarchy` 函式連接 L3。

5. **新增命令列選項**  
   在 `configs/common/Options.py` 中加入 `--l3cache` 選項，以啟用 L3 模組。

6. **混合編譯 GEM5 + NVMain**  
   在 gem5 根目錄使用以下指令：
   ```bash
   scons EXTRAS=../NVmain build/X86/gem5.opt
7. **模擬測試**
   ```bash
   ./build/X86/gem5.opt configs/example/se.py -c tests/test-progs/hello/bin/x86/linux/hello --cpu-type=TimingSimpleCPU --caches --l2cache --l3cache --mem-type=NVMainMemory --nvmain-config=../NVmain/Config/PCM_ISSCC_2012_4GB.config > terminal_output.txt
   > i0.defaultMemory.channel0.FRFCFS-WQF.channel0.rank0.bank0.subarray0.activeEnergy = 4.1412nJ
