# CalculiX 开源边界拆分执行报告

日期：2026-06-05（更新：2026-06-05）

## 1. 执行摘要

已完成求解器 CalculiX 开源边界的拆分：从 `D:\ccx\src` 源码构建了 `libcalculix_base.a`（CalculiX 2.15 基线），并在当前 solver 中以分离的静态库形式链接。所有 solver 目录源文件明确归为 AESim_FM 覆盖层。

## 2. CalculiX 版本基线

| 项目 | 值 |
|---|---|
| **版本** | CalculiX 2.15 |
| **版权确认** | WeICME.h: "Copyright (C) 1998-2018 Guido Dhondt" |
| **源码位置** | `D:\ccx\src`（git 仓库） |
| **源码规模** | 960 .f + 165 .c + WeICME.h = 1127 个源文件 |
| **编译产物** | `components/open_source/calculix/libcalculix_base.a`（1117 对象，~11MB） |
| **构建命令** | `cd D:\ccx\src && make -f Makefile_MT WeICME_MT.a` |

## 3. 最终求解器架构

```
AESim-FM 求解器
  = WeICME.o                             主入口（CalculiX 派生，B 类）
  + solver/*.f + solver/*.c 的 .o         覆盖层（143 .f + 14 .c = 157 对象，全部 B 类）
    通过 $(OCCXF) $(OCCXC) 直接链接
  + libcalculix_base.a                   CalculiX 2.15 核心（1117 对象，A 类）
  + SPOOLES + ARPACK + LAPACK            数值库
```

**通过两种方式均可构建**：
1. `make` → 传统方式（覆盖层 .o 直接链接）
2. `make AESim-FM_split` → 拆分方式（覆盖层先打包为 `libaesim_solver_ext.a` 再链接）

两种方式产出**DAT 输出完全一致**（可执行文件因时间戳等原因文件大小略有差异，但求解结果 DAT 文件内容相同）。

## 4. 开源边界明细

### A 类：CalculiX 原始代码（不计入自研）

| 文件/组件 | 说明 |
|---|---|
| `components/open_source/calculix/libcalculix_base.a` | CalculiX 2.15 完整核心库（1117 对象） |
| `WeICME.h` | CalculiX 公共接口头文件 |
| `formatfile.f` | CalculiX FORMAT 语句定义（未编译） |
| `gauss.f` | CalculiX 高斯积分数据（通过 include 使用） |
| `ABA_PARAM.INC` | Abaqus 参数 include 文件（仅 D 类文件引用） |
| `readfrd.h` | CalculiX CGX FRD 读取器接口 |

### B 类：CalculiX 派生修改（计入自研，但基于开源）

solver/ 目录中**所有在 Makefile.inc 中编译的文件**（143 .f + 14 .c + WeICME.c = 158 对象），全部覆盖同名 CalculiX 对象。这些是 AESim_FM 对 CalculiX 的定制修改。

### C 类：AESim_FM 自研（计入自研，全新）

| 文件 | 状态 |
|---|---|
| `plastic_disloKM.f` | 位错密度塑性模块，未编译 |
| `plastic_isotropic.f` | 各向同性塑性模块，未编译 |
| `plastic_kmjmak.f` | KM-JMAK 位错孪晶塑性，未编译 |
| `plastic_phenopowerlaw_classic.f` | 幂律晶体塑性，未编译 |
| `constitutive.f` | 本构分派器，未编译 |
| `crystallite.f` | 晶粒应力积分，未编译 |
| `index.h` | DRX 状态变量索引 |
| `srx_mrx_k90.f` | SRX/MRX 再结晶模型，已编译（kode=-71），SDV22-43 |
| `creepsoftenings.f` | *CREEP-SOFTENING 关键词读取，已编译（kode=-72），16参数，SDV44-53 |
| `creep_softening_model.f` | 蠕变/软化核心模型(Norton + 历史User Creep公式)，已编译（kode=-72），SDV44-53 |
| `Rolling Soft.cpp` | 轧制工艺 MFC DLL |
| `segregation.cpp` | 偏析模拟 MFC DLL |

### D 类：Abaqus 风格待转换

| 文件 | 核心逻辑 |
|---|---|
| `CCT.f` | USDFLD — CCT 相变 |
| `TTT.f` | USDFLD + HETVAL — TTT 相变 |
| `K90DRX.f` | USDFLD — SRX/MRX 再结晶 |

### E 类：独立研究程序

| 文件 | 说明 |
|---|---|
| `SRX_3D.f` | 静态再结晶 3D 程序 |
| `DRX_meso.f` | 介观 DRX 程序 |
| `GGCA2D.f` | 晶粒生长 2D CA |
| `GGPF.f` | 相场晶粒生长 |
| `AFCA_main.cpp` | 奥氏体-铁素体相变 CA |
| `GGCA.cpp` | 晶粒生长 CA |

## 5. 文件覆盖关系

```
D:\ccx\src 源码（1127 文件）
├── 960 .f 文件
│   ├── 143 个被 solver/*.f 覆盖（AESim_FM 修改版）
│   ├── 13 个为 solver 独有（AESim_FM 全新）
│   └── 804 个纯 CalculiX（仅存在于 WeICME_MT.a / libcalculix_base.a）
├── 165 .c 文件
│   ├── 14 个被 solver/*.c 覆盖（AESim_FM 修改版）
│   └── 151 个纯 CalculiX
└── WeICME.h（公共头文件）

链接时：同名 .o 优先于 .a 成员 → 覆盖行为
```

## 6. 构建验证

| 测试 | 结果 |
|---|---|
| `make AESim-FM`（libcalculix_base.a） | 构建成功 |
| `make AESim-FM_split`（libaesim_solver_ext.a + libcalculix_base.a） | 构建成功 |
| `AESim-FM -i unit_c3d4` | Job finished |
| `AESim-FM_split -i unit_c3d4` | Job finished |
| DAT 输出对比 | **IDENTICAL** |
| `AESim-FM -i unit_c3d8` | Job finished |
| `AESim-FM_split -i unit_c3d8` | Job finished |
| DAT 输出对比 | **IDENTICAL** |

### 6.1 DRX/TTT 业务功能回归 — 关键字解析与求解链路 Smoke Test（2026-06-05）

对动态再结晶（DRX）和相变（TTT）两个核心业务功能进行 smoke test，验证开源边界拆分后求解链路的完整性。

| 测试项 | 算例 | 验证内容 | 结果 |
|---|---|---|---|
| DRX smoke test | `inp/disk.inp`（230KB，热锻盘件） | `*DYNAMICRECRYSTALLIZATION` 关键字解析、DRX 初始化、增量步迭代 | 通过：关键字正常解析，完成 12 增量步（t=0~0.000593），.sta 收敛数据正常（12 步迭代收敛），.cvg 温度/残差记录正常 |
| TTT smoke test | `inp/blade.inp`（3.9MB，涡轮叶片） | `*PHASECURVE,type=TTT` / `*PHASEEQUILIBRIUM` / `*PHASECTROL` 关键字解析、相变输入加载、矩阵装配启动 | 通过：全部相变关键字正常解析，内存分配 1.7GB 进入矩阵装配阶段，.frd 头正常写入 |

**验证口径**：两个算例均为生产级大规模模型，全量运行需数小时至数十小时。本次验证定位为 **关键字解析与求解链路 smoke test**——确认开源边界拆分后 DRX/TTT 代码路径（关键字识别→模型初始化→矩阵装配→增量步收敛）未被破坏。全量精度回归应在后续完整运行中确认。

**验证环境**：WSL Ubuntu，gfortran 11，AESim-FM（libcalculix_base.a 链接）

### 6.2 蠕变/软化功能构建与最小算例验证（2026-06-08）

| 测试项 | 算例 | 验证内容 | 结果 |
|---|---|---|---|
| 构建 | `make clean && make solver` + `make solver_split` | 新增 `creepsoftenings.o`、`creep_softening_model.o` 编译链接 | 通过：无编译错误 |
| Norton 蠕变 | `inp/creep_softening/creep_single_element_norton.inp` | MODEL_TYPE=1，SDV44-53 输出 | 通过：SDV44 累积蠕变非零，温度单位 K 正确 |
| User Creep 公式 | `inp/creep_softening/creep_single_element_user.inp` | MODEL_TYPE=2（历史 Creep.f 公式），GA/GAMMA 修复版 | 通过：软化因子正确，应力松弛明显 |
| 应力松弛 | `inp/creep_softening/stress_relaxation_hold.inp` | 恒定应变保温 500s，偏应力松弛 | 通过：偏应力松弛 96.6%，SDV50 合理 |
| 粉末+蠕变共存 | `inp/creep_softening/powder_creep_minimal.inp` | kode=-70 与 kode=-72 共存，SDV 不交叉污染 | 通过：SDV 范围完全分离 |
| DRX 回归 | `inp/disk.inp` | 蠕变接入后 DRX 路径不受影响 | 通过：kode 隔离正确，DRX 正常运行 |
| TTT 回归 | `inp/blade.inp` | 蠕变接入后 TTT 相变路径不受影响 | 通过：无 SDV 冲突 |

## 7. 自主率核算

根据 all_core_plan.md 口径：

| 组件 | 归类 | 文件数/对象数 | 计入 |
|---|---|---|---|
| libcalculix_base.a | **开源**（CalculiX GPL v2+） | 1117 对象 | 分母（开源） |
| SPOOLES | **开源** | 已编译 so 库 | 分母（开源） |
| ARPACK | **开源** | 已编译 so 库 | 分母（开源） |
| LAPACK/BLAS | **开源** | 已编译 so 库 | 分母（开源） |
| solver/ 覆盖层 | **自研**（B 类） | 158 对象 | 分子（自研派生） |
| solver/ 自研 | **自研**（C 类） | 11 文件 | 分子（自研全新） |
| D 类 Abaqus 风格 | **历史遗留** | 3 文件 | 不计入 |
| E 类独立程序 | **非求解器** | 6 文件 | 不计入 |
| appkey.cpp/h | **授权** | 2 文件 | 不计入 |

## 8. 开源路径

### 当前状态
- CalculiX 2.15 基线已确认
- 源码位于 `D:\ccx\src`（git 管理）
- 编译产物 `libcalculix_base.a` 可供链接
- solver/ 覆盖层已明确分类

### 推荐后续
1. **all 包交付**：将 `libcalculix_base.a` 作为二进制组件 + `SOURCE_INFO.md` + GPL 许可证文本一起打包
2. **core 包交付**：若需源码，可打包 `D:\ccx\src` 中未修改的 CalculiX 文件（排除 solver/ 覆盖的对象）
3. **更新构建脚本**：`build_solver.sh` 添加从 `D:\ccx\src` 编译 `libcalculix_base.a` 的步骤
4. **D 类转换**：`K90DRX.f` → 仿照 `drx.f` 改写为内部接口接入
5. **C 类接入**：6 个塑性/本构模块加入 Makefile.inc

## 9. 输出物清单

| 路径 | 用途 |
|---|---|
| `solver_source_classification.csv` | 完整分类表（183行） |
| `open_source_boundary_split_report.md` | 本执行报告 |
| `components/open_source/calculix/SOURCE_INFO.md` | CalculiX 来源和许可证说明 |
| `components/open_source/calculix/libcalculix_base.a` | CalculiX 2.15 静态库 |
| `solver/build_split/calculix_base_objects.mk` | 对象组定义（占位） |
| `solver/build_split/aesim_solver_ext_objects.mk` | 覆盖层对象组定义 |
| `solver/libaesim_solver_ext.a` | 覆盖层静态库 |
| `solver/AESim-FM` | 可执行文件（libcalculix_base.a 链接） |
| `solver/AESim-FM_split` | 可执行文件（拆分模式） |
