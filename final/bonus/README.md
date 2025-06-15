# Bonus: Design Last Level Cache Policy to Reduce Energy Consumption of PCM-Based Main Memory

## 說明

本題目目標為在 GEM5 中設計一個能降低PCM能耗的 Last Level Cache (L3) 替換策略。
本策略以原始的 `base_set_assoc` 為基礎，修改為 Dirty-Aware Policy，優先替換乾淨的區塊。

## 實作步驟

1. **baseline模擬測試**      
   replace policy 預設為 LRU，`--l3_assoc` 設為 2，執行以下指令：(Q3 2-way的結果)
   ```bash
   ./build/X86/gem5.opt configs/example/se.py -c benchmark/quicksort --cpu-type=TimingSimpleCPU --caches --l2cache --l3cache --l3_assoc=2 --l1i_size=32kB --l1d_size=32kB --l2_size=128kB --l3_size=1MB --mem-type=NVMainMemory --nvmain-config=../NVmain/Config/PCM_ISSCC_2012_4GB.config > terminal_output.txt
   ```

   > i0.defaultMemory.channel0.FRFCFS-WQF.channel0.rank0.bank0.subarray0.activeEnergy = 781.55nJ


2. **實作 findVictim 函數邏輯**  
   ```python
   CacheBlk*
   LLCDirtyAware::findVictim(Addr addr)
   {
       std::vector<CacheBlk*> locations = getPossibleLocations(addr);

       CacheBlk* clean_candidate = nullptr;
       CacheBlk* fallback_candidate = nullptr;

       for (auto* blk : locations) {
           if (!blk->isValid()) {
               return blk;  // find an empty one, just use it
           }

           if (!blk->isDirty() && clean_candidate == nullptr) {
               clean_candidate = blk;
           }

           if (fallback_candidate == nullptr) {
               fallback_candidate = blk;
           }
        }

        if (clean_candidate != nullptr) {
            DPRINTF(CacheRepl, "set %x, way %x: selecting clean blk for replacement\n",
                clean_candidate->set, clean_candidate->way);
            return clean_candidate;
        }

        DPRINTF(CacheRepl, "set %x, way %x: selecting dirty blk for replacement\n",
            fallback_candidate->set, fallback_candidate->way);
        return fallback_candidate;
    }
    ```

3. **混合編譯 GEM5 + NVMain**  
   在 `gem5` 根目錄使用以下指令：
   ```bash
   scons EXTRAS=../NVmain build/X86/gem5.opt

4. **模擬測試**  
   將 `--l3_assoc` 設為 2，執行以下指令：
   ```bash
   ./build/X86/gem5.opt configs/example/se.py -c benchmark/quicksort --cpu-type=TimingSimpleCPU --caches --l2cache --l3cache --l3_assoc=2 --l1i_size=32kB --l1d_size=32kB --l2_size=128kB --l3_size=1MB --mem-type=NVMainMemory --nvmain-config=../NVmain/Config/PCM_ISSCC_2012_4GB.config > terminal_output.txt
   ```
 
   > i0.defaultMemory.channel0.FRFCFS-WQF.channel0.rank0.bank0.subarray0.activeEnergy = 672.904nJ

5. **結果分析**  
   開啟 `terminal_output.txt`，比較兩種組合度設定下的 activeEnergy，在 quicksort 擴增為 `500000` 的情況下:
    - LRU: 781.55nJ，activeEnergy 較高
    - LRU + Dirty-Aware: 672.90nJ，activeEnergy 較低





