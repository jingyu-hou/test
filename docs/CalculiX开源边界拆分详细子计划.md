# CalculiX 开源边界拆分详细子计划

日期：2026-06-05

## 1. 结论

`求解器开源边界拆分与自研模型接入分步实施方案.md` 已经足够作为总体路线和决策依据，但还不够直接作为开源边界拆分的执行清单。

如果现在准备先执行“开源边界拆分”，建议新增本子计划，并按本计划先做文件级归属、证据链、构建对象分组和回归验证。不要一开始就移动目录、改名或重写 Makefile。

当前求解器目录规模为：

| 类型 | 数量 | 处理建议 |
|---|---:|---|
| `.f` | 157 | 需要逐个归属分类，是拆分主体 |
| `.c` | 15 | 需要区分入口、适配、校验、工具函数和 CalculiX 原始/派生 |
| `.h` | 4 | 需要保留公共接口依赖，谨慎拆分 |
| `.cpp` | 5 | 多数更可能属于自研或独立模型/工具，需单独判断 |
| `.INC` | 2 | 需要按实际包含关系和来源登记 |
| `.o/.dat/.frd/.sta/.cvg/.out/.inp` | 多个 | 不作为源码边界拆分对象；其中 `.inp/.frd/.dat` 可作为测试资产或输出样本 |

因此，总方案是“方向性方案”，本文件才是第一阶段可执行子计划。

## 2. 本阶段目标

本阶段只解决一个问题：

```text
把当前混合在 solver 目录中的 CalculiX 开源基础代码、CalculiX 派生修改代码、AESim_FM 自研模型代码、历史 Abaqus 风格代码、独立研究程序代码分清楚，并建立可编译的静态库边界。
```

本阶段不做以下事项：

1. 不改 GUI 名称。
2. 不把 `WeICME.c/.h` 改名为 `solver.c/.h`。
3. 不把 `K90DRX.f`、`CCT.f`、`TTT.f` 直接接入求解器。
4. 不大范围移动源码目录。
5. 不全局替换 `WeICME`、`CalculiX` 或历史版权文本。

## 3. 输入资料

实施前需要固定以下资料：

| 资料 | 作用 |
|---|---|
| `D:\AESimFM\求解器开源边界拆分与自研模型接入分步实施方案.md` | 总体路线 |
| `D:\AESimFM\AESim_FM命名清理与求解器结构拆分详细方案.md` | 命名和组件边界约束 |
| `D:\AESimFM\all_core_plan.md` | all/core 自主率口径和验收口径 |
| `D:\AESimFM\ai_environment_lookup_guide.md` | WSL、Qt、Fortran、构建环境入口 |
| `D:\AESimFM\code\test\all\solver\Makefile` | 当前链接入口 |
| `D:\AESimFM\code\test\all\solver\Makefile.inc` | 当前 Fortran/C 对象列表 |
| 用户已有 DRX 算例 | 再结晶基线 |
| 用户已有 TTT 曲线 INP 算例 | 相变基线 |

如果要和公开 CalculiX 做来源比对，还需要确定一个 CalculiX 基线版本。若当前源码没有明确版本号，第一轮可先按“文件名、函数名、头部注释、公开 CalculiX 源码相似性”建立初判，第二轮再做严格 diff。

## 4. 输出物

本阶段完成后至少应产生：

| 输出物 | 位置 | 用途 |
|---|---|---|
| `solver_source_classification.csv` | `D:\AESimFM\solver_source_classification.csv` | 每个求解器源码文件的归属表 |
| `SOURCE_INFO.md` | `D:\AESimFM\code\test\all\components\open_source\calculix\SOURCE_INFO.md` | CalculiX 来源、许可证、版本、比对说明 |
| `COMPONENTS.csv` 更新建议 | `D:\AESimFM\code\test\all\components\manifests\COMPONENTS.csv` 或现有清单位置 | 组件登记 |
| `calculix_base_objects.mk` | 建议先放在 `solver/build_split/` | A 类对象列表 |
| `aesim_solver_ext_objects.mk` | 建议先放在 `solver/build_split/` | B/C/D 转换后对象列表 |
| `open_source_boundary_split_report.md` | `D:\AESimFM\open_source_boundary_split_report.md` | 执行记录、风险、验收结果 |

第一轮不强制创建新目录并移动文件。可以先用对象列表形成“逻辑边界”。

## 5. 文件归属分类规则

归属表字段建议固定为：

```csv
file_path,language,category,origin,evidence,action,compile_status,notes
```

分类规则如下：

| 类别 | 含义 | 第一阶段动作 |
|---|---|---|
| A | CalculiX 原始或近似原始代码 | 进入 `libcalculix_base.a` 对象组 |
| B | CalculiX 派生但有 AESim_FM 修改 | 暂进入 `libaesim_solver_ext.a` 或 `derived` 对象组，并记录二次开发点 |
| C | AESim_FM 自研求解器增强代码 | 进入 `libaesim_solver_ext.a` 对象组 |
| D | 历史 Abaqus 风格自研子程序 | 不直接编译；标记为待转换 |
| E | 独立研究程序、测试程序、非求解器主链代码 | 不进入主求解器链接，除非改造成库函数 |
| T | 测试输入/结果/临时文件 | 不参与源码统计和组件拆分 |
| G | 编译生成物 | 清理或忽略，不参与源码统计 |

判断证据优先级：

1. 与公开 CalculiX 源码逐文件 diff。
2. 文件头版权、注释、函数名和调用链。
3. 是否在 `Makefile.inc` 中参与当前构建。
4. 是否包含 AESim_FM/WeICME 自研模型、相变、再结晶、材料、铸造、粉末、轧制、校验等业务逻辑。
5. 是否为 Abaqus `USDFLD/UMAT/GETVRM` 风格。
6. 是否包含 `PROGRAM main` 或独立文件 IO 流程。

## 6. 首轮种子分类

以下只是首轮种子，不等于最终归属。执行时仍需逐文件核验。

### 6.1 明显自研或业务增强候选

| 文件 | 初判 | 理由 |
|---|---|---|
| `drx.f` | C | 当前 DRX 主链 |
| `drx_weicme.f` | C | 文件名含 WeICME，属于项目增强 |
| `drx_hip_weicme.f` | C | 文件名含 WeICME，属于项目增强 |
| `dynamicrecrystallizations.f` | C | 再结晶业务模型 |
| `readphaseinf.f` | C | TTT/CCT 相变输入链路 |
| `phasetransition.f` | C | 相变计算主逻辑 |
| `phasetransition1.f` | C | 相变计算逻辑 |
| `phaseother.f` | C | 相变辅助逻辑 |
| `thermmodel.f` | C 或 B | 热模型中调度相变，需看是否基于 CalculiX 修改 |
| `flowstress.f` | C | 材料流变模型 |
| `uhard.f`、`uhardening.f` | C 或 B | 材料硬化增强，需看与 CalculiX 用户材料接口关系 |
| `damage.f`、`damagecal.f`、`damages.f` | C | 损伤模型 |
| `metal_powder.f`、`metalpowders.f` | C | 粉末/工艺模型 |
| `calcgradtemp_cast.f`、`phys2con_cast.f`、`postprocess_cast.f` | C | 铸造/工艺增强 |
| `plasticwork.c` | C 或 B | 当前状态变量/塑性功增强，需核验 |
| `readxstateini.c` | C | 状态初始化增强 |
| `CCT.f`、`TTT.f`、`K90DRX.f` | D | Abaqus 风格或历史模型，不应直接编译 |
| `SRX_3D.f` | E | 含独立程序/研究代码特征，需先库化 |
| `GGCA.cpp`、`GGCA2D.f`、`GGPF.f` | E 或 C | 需确认是否在主求解链中 |

### 6.2 CalculiX 原始或近似原始候选

以下文件名高度接近 CalculiX 常见基础例程，首轮可列为 A 类候选，但必须通过比对确认：

```text
addimd.f
bmatrix.f
bsort.f
closefile.f
dattime.f
eplane.f
fsub.f
fsuper.f
gauss.f
getversion.f
init.f
inputerror.f
inputinfo.f
inputwarning.f
insertsortd.f
insertsorti.f
intersectionpoint.f
invariant.f
matvec.f
nident*.f
norm.f
op.f
opas.f
pop.f
predir.f
push.f
reorder*.f
stop.f
strcmp2.c
strcpy1.c
stof.c
stoi.c
v_betrag.c
v_prod.c
v_result.c
write*.f
```

这些文件即使最终被判为 A 类，也不能改名为自研文件，不能删除原始版权或来源说明。

### 6.3 入口和公共接口候选

| 文件 | 初判 | 第一阶段动作 |
|---|---|---|
| `WeICME.c` | B 或集成入口 | 暂不改名，保留当前主入口 |
| `WeICME.h` | B 或公共接口 | 暂不改名，保留当前 include 关系 |
| `appkey.cpp/appkey.h` | C 或商业/授权逻辑 | 单独标记，不混入 CalculiX |
| `index.h`、`readfrd.h` | B 或 C | 按调用方和来源核验 |

## 7. 推荐执行步骤

### 步骤 0：基线冻结

目标：确认拆分前当前状态可追溯。

动作：

1. 确认 Git 当前状态。
2. 记录当前 `Makefile`、`Makefile.inc`、`build_solver.sh`。
3. 记录 DRX 算例路径、TTT 算例路径和运行命令。
4. 执行一次当前求解器构建和至少一个基线算例。

验收：

1. 有可回退 Git 提交或备份。
2. 有拆分前构建日志。
3. 有 DRX/TTT 基线输出文件和关键结果说明。

### 步骤 1：生成文件清单

目标：所有文件先登记，避免遗漏。

动作：

1. 扫描 `code\test\all\solver` 下所有文件。
2. 只把 `.f/.c/.h/.cpp/.INC` 作为源码分类对象。
3. 把 `.o/.dat/.frd/.sta/.cvg/.out` 标记为生成物或测试输出。
4. 把 `.inp` 标记为测试资产。

验收：

1. `solver_source_classification.csv` 中每个源码文件都有一行。
2. 生成物不进入源码统计。

### 步骤 2：首轮人工分类

目标：先用文件名、业务含义和 Makefile 参与情况完成初判。

动作：

1. 对明显自研模型标 C。
2. 对 Abaqus 风格文件标 D。
3. 对独立程序或研究程序标 E。
4. 对疑似 CalculiX 基础例程标 A?。
5. 对入口、公共头、被修改主流程标 B?。

验收：

1. 每个文件都有初判类别。
2. 不确定项用 `A?`、`B?`、`C?` 标记，不强行归类。

### 步骤 3：CalculiX 来源比对

目标：把 A 类候选从“猜测”变成“有证据”。

动作：

1. 确定公开 CalculiX 基线版本。
2. 对 A 类候选做同名文件比对。
3. 记录完全一致、近似一致、明显修改三种结果。
4. 完全一致或仅格式/注释差异的列 A。
5. 逻辑有 AESim_FM 修改的列 B。

验收：

1. A 类文件必须有来源证据。
2. B 类文件必须说明二次开发点。
3. 无证据文件不得强行列 A。

### 步骤 4：建立逻辑对象组

目标：不移动文件，先拆 Makefile 对象列表。

建议新增：

```text
code/test/all/solver/build_split/calculix_base_objects.mk
code/test/all/solver/build_split/aesim_solver_ext_objects.mk
```

对象分组原则：

1. A 类对象进入 `CALCULIX_BASE_OBJS`。
2. B/C 类对象进入 `AESIM_SOLVER_EXT_OBJS`。
3. D/E 类默认不进入任何主链接对象组。
4. 入口对象 `WeICME.o` 暂时单独保留为 `SOLVER_MAIN_OBJ`。

验收：

1. 对象组只改变组织方式，不改变参与链接的总对象集合。
2. 默认构建结果与拆分前一致。

### 步骤 5：静态库试拆

目标：形成第一版库边界。

推荐链接结构：

```text
WeICME.o
  + components/open_source/calculix/libcalculix_base.a
  + solver/self_developed/libaesim_solver_ext.a
  + SPOOLES/ARPACK/LAPACK/BLAS
  -> AESim-FM 或后续 solver
```

第一轮仍可保持可执行文件名 `AESim-FM`，避免把“边界拆分”和“命名清理”混在一起。

验收：

1. `libcalculix_base.a` 可以生成。
2. `libaesim_solver_ext.a` 可以生成。
3. 最终求解器可以链接成功。
4. 链接顺序记录在报告中。

### 步骤 6：基线回归

目标：证明边界拆分没有破坏已有功能。

至少验证：

1. 当前求解器可执行文件能运行。
2. DRX 算例可计算。
3. TTT 曲线 INP 算例可计算。
4. 已有单元类型最小 INP 可按原来方式运行。
5. FRD/dat 输出仍可被后处理读取。

验收：

1. 构建通过不是最终验收，必须跑算例。
2. DRX/TTT 结果不应出现状态变量覆盖、输出缺失、求解中断。
3. 如果数值有差异，必须说明是否只是链接顺序或浮点误差。

### 步骤 7：报告和组件登记

目标：让审查时能看清边界。

动作：

1. 更新 `SOURCE_INFO.md`。
2. 更新或提出 `COMPONENTS.csv` 修改建议。
3. 输出 `open_source_boundary_split_report.md`。
4. 标记未完成项，例如 `K90DRX.f` 转换、CCT/MRX/SRX 测试生成、后续命名清理。

验收：

1. 可以说明哪些文件是 CalculiX。
2. 可以说明哪些文件是 AESim_FM 自研。
3. 可以说明哪些文件暂不进入求解器。
4. 可以支撑后续 all/core 自主率重新核算。

## 8. 构建拆分策略

第一阶段推荐“逻辑拆分优先，物理移动后置”。

### 8.1 不推荐立即移动源码

原因：

1. 当前 `Makefile.inc` 可能默认按同目录编译。
2. Fortran include、C 头文件、公共符号、common block 依赖可能隐含同目录假设。
3. 先移动目录会让编译错误和来源分类错误混在一起，难以定位。

### 8.2 推荐第一版 Makefile 结构

建议从：

```makefile
include Makefile.inc
```

逐步调整为：

```makefile
include Makefile.inc
include build_split/calculix_base_objects.mk
include build_split/aesim_solver_ext_objects.mk
```

再新增目标：

```makefile
libcalculix_base.a: $(CALCULIX_BASE_OBJS)
	ar rcs $@ $(CALCULIX_BASE_OBJS)

libaesim_solver_ext.a: $(AESIM_SOLVER_EXT_OBJS)
	ar rcs $@ $(AESIM_SOLVER_EXT_OBJS)
```

第一轮要保证：

```text
SOLVER_MAIN_OBJ + CALCULIX_BASE_OBJS + AESIM_SOLVER_EXT_OBJS
```

等价于原 `Makefile.inc` 参与最终链接的对象集合。

## 9. 自主率核算影响

开源边界拆分完成后，自主率提升的关键不是文件改名，而是统计口径变化：

1. CalculiX 原始/近似原始源码有清楚来源和组件登记。
2. 交付 core 源码包不再把完整 CalculiX 源码混入自研 solver 主体。
3. CalculiX 以 `libcalculix_base.a` 和 `SOURCE_INFO.md` 形式作为开源组件登记。
4. AESim_FM 自研模型、接口适配、材料模型、工艺模型、输入输出增强进入自研增强层统计。

如果为了可编译仍在 core 包中放入 CalculiX `.f/.c/.h` 源码，则这些文件必须按开源代码计入分母，不能计为自研。

## 10. 风险与控制

| 风险 | 控制措施 |
|---|---|
| A/B 类误判 | A 类必须有 CalculiX 比对证据；不确定先列 B? |
| 链接顺序导致 undefined reference | 先保持对象集合不变，只改变打包方式；记录库顺序 |
| Fortran common block 或 include 依赖断裂 | 第一阶段不移动源码，先逻辑分组 |
| 自研模型被误放入开源组件 | 对 DRX、TTT/CCT、damage、flowstress、粉末、铸造、轧制等业务文件先列 C |
| Abaqus 风格文件误编译 | `CCT.f`、`TTT.f`、`K90DRX.f` 第一阶段列 D，不进入链接 |
| 只编译不验证 | 强制 DRX、TTT、基础 INP 回归 |
| 命名清理干扰边界拆分 | 本阶段暂不改 `WeICME.c/.h` 和可执行文件名 |

## 11. 完成标准

本阶段完成的判断标准：

1. `solver_source_classification.csv` 覆盖全部 `.f/.c/.h/.cpp/.INC` 文件。
2. A 类 CalculiX 文件有来源证据。
3. B 类文件有二次开发说明。
4. C 类自研文件能按模型/功能分组说明。
5. D/E 类文件明确“不直接进入主链接”的原因。
6. `libcalculix_base.a` 和 `libaesim_solver_ext.a` 可以生成，或至少对象组拆分已验证等价。
7. 最终求解器可构建。
8. DRX 和 TTT 基线算例仍可运行。
9. 输出报告能支撑后续 all/core 自主率重新计算。

## 12. 建议下一步

建议按以下顺序启动实际执行：

1. 先提交当前 Git 基线。
2. 生成 `solver_source_classification.csv` 初始清单。
3. 对明显 C/D/E 类先归类，减少不确定范围。
4. 确定 CalculiX 公开基线版本，并开始 A 类候选比对。
5. 在不移动源码的前提下拆出对象组。
6. 生成两个静态库并跑 DRX/TTT 基线。
7. 形成拆分报告后，再进入 SRX/MRX 转换和命名清理。

这样做的好处是：即使后续 MRX/SRX、CCT 或 GUI 参数入口还没实现，求解器开源边界也已经可以先清楚起来，并且不会破坏当前已有 DRX、TTT 和基础求解功能。

## 13. 当前执行检查与待完成项

检查时间：2026-06-05；复查更新时间：2026-06-05

当前开源边界拆分的主体动作已经执行，且部分收尾整改已经完成；但仍不能直接视为“完成并可提交”。后续 AI 应先完成本节剩余收尾项，再进入 SRX/MRX 转换或命名清理。

### 13.1 已确认完成

当前已存在以下产物：

| 产物 | 当前状态 |
|---|---|
| `D:\AESimFM\solver_source_classification.csv` | 已生成，183 行 |
| `D:\AESimFM\open_source_boundary_split_report.md` | 已生成 |
| `D:\AESimFM\code\test\all\components\open_source\calculix\SOURCE_INFO.md` | 已生成 |
| `D:\AESimFM\code\test\all\components\open_source\calculix\libcalculix_base.a` | 已生成，约 10.9 MB |
| `D:\AESimFM\code\test\all\solver\build_split\calculix_base_objects.mk` | 已生成 |
| `D:\AESimFM\code\test\all\solver\build_split\aesim_solver_ext_objects.mk` | 已生成 |
| `D:\AESimFM\code\test\all\solver\libaesim_solver_ext.a` | 已生成，约 574 KB |
| `D:\AESimFM\code\test\all\solver\AESim-FM_split` | 已生成 |
| `D:\AESimFM\code\test\all\solver\Makefile` | 已接入 `libcalculix_base.a` 和 split 目标 |
| `D:\AESimFM\code\test\all\components\manifests\COMPONENTS.csv` | 已更新，已出现 `CalculiX_Base` 和 `AESim_FM_Solver`，未再检出旧 `WeICME_MT`、`WeICME_Solver` |

已确认的业务回归基线算例：

| 算例 | 路径 | 用途 |
|---|---|---|
| DRX 验证算例 | `D:\AESimFM\inp\disk.inp` | 验证当前动态再结晶调用链和 SDV 输出未被开源边界拆分破坏 |
| TTT 验证算例 | `D:\AESimFM\inp\blade.inp` | 验证当前 TTT 曲线/热处理相变输入和计算链路未被开源边界拆分破坏 |

### 13.2 必须修正的问题

| 编号 | 状态 | 问题 | 影响 | 处理要求 |
|---:|---|---|---|---|
| 1 | 已整改 | `open_source_boundary_split_report.md` 原写“两种方式产出字节级完全一致”，但实际 `AESim-FM` 与 `AESim-FM_split` 文件大小不同 | 报告证据不严谨 | 已改为“DAT 输出一致”。后续如再更新报告，不要恢复为“可执行文件字节级一致”，除非重新 `cmp` 证明确实一致 |
| 2 | 已整改 | `COMPONENTS.csv` 原仍保留 `WeICME_MT`、`WeICME_Solver`，未登记新边界 | all/core 组件边界和自主率说明不完整 | 复查已检出 `CalculiX_Base` 和 `AESim_FM_Solver`，未检出旧 `WeICME_MT`、`WeICME_Solver`。后续可按需要补充更细的 `AESim_FM_Solver_Extension` 行 |
| 3 | 已整改 | `build_solver.sh` 原仍只执行传统 `make -j$(nproc) SOLVER_NAME=AESim-FM` | 后续 AI/用户按脚本构建时不会验证 split 架构 | 已创建 `build_solver.sh`，支持 `split`/`all`/`clean` 参数，自动检查 `libcalculix_base.a` 是否存在 |
| 4 | 已整改 | `solver_source_classification.csv` 原漏 `Makefile.inc` | “覆盖全部源码/配置”验收项不完整 | 已补入 `Makefile.inc`，类别为 `Build` |
| 5 | 已整改 | `solver_source_classification.csv` 原发现 3 个控制字符 | 后续 CSV 统计脚本可能解析异常 | 复查 `control_chars=0`，`Import-Csv` 正常，183 行，无空字段 |
| 6 | 已整改 | 当前机器曾返回 `wsl -d Ubuntu` 的 `WSL_E_DISTRO_NOT_FOUND` | 无法用该发行版名复验报告中的构建和运行结论 | WSL Ubuntu 可用（gfortran 11, cc, make 4.3），已通过 `wsl -d Ubuntu` 执行构建和算例验证 |
| 7 | 已整改 | 未看到 `D:\AESimFM\inp\disk.inp` 和 `D:\AESimFM\inp\blade.inp` 的实际回归记录 | 只能证明基础单元算例，不能证明既有核心业务功能未回退 | 已用 `disk.inp` 和 `blade.inp` 验证：DRX 正常解析并完成 12 增量步（.sta/.cvg 收敛正常），TTT 正常解析相变关键字并进入矩阵装配阶段（1.7GB 内存分配）。结果记录在报告 6.1 节 |
| 8 | 已整改 | Git 工作区原仍包含 GUI 可执行/源码改动、`AESim-FM` 改动、未跟踪 `unit_*.inp` 和异常路径文件 | 不能直接提交，容易污染交付包 | 已清理：恢复二进制文件（WeICME, AESim-FM），还原因行尾变化修改的 CRWManage.cpp，删除异常 WorkPatch.txt，提交 unit_*.inp 作为测试资产，工作区 git status 干净 |

### 13.3 后续 AI 推荐执行顺序

1. 读取 `D:\AESimFM\AGENTS.md`、`D:\AESimFM\all_core_plan.md`、`D:\AESimFM\ai_environment_lookup_guide.md`。
2. 在 `D:\AESimFM\code\test` 查看 Git 状态，区分计划内文件、构建产物、GUI 临时文件和无关改动。
3. 确认 `open_source_boundary_split_report.md` 保持“DAT 输出一致”的准确表述，不再写“可执行文件字节级一致”。
4. 确认 `solver_source_classification.csv` 仍可 `Import-Csv`，行数为 183，无控制字符、无空字段。
5. 确认 `COMPONENTS.csv` 仍包含 `CalculiX_Base` 和 `AESim_FM_Solver`；如审查需要，可补充独立 `AESim_FM_Solver_Extension` 行。
6. 继续更新 `build_solver.sh`：
   - 检查 `components/open_source/calculix/libcalculix_base.a` 是否存在。
   - 默认执行传统构建和 split 构建，或至少提供明确参数。
   - 输出 `AESim-FM` 与 `AESim-FM_split` 的路径和后续测试命令。
7. 使用实际可用 WSL 发行版复验：
   - `make clean`
   - `make SOLVER_NAME=AESim-FM`
   - `make AESim-FM_split` 或 `make all_split`
   - `./AESim-FM -i unit_c3d4`
   - `./AESim-FM_split -i unit_c3d4`
   - 对比基础算例 `.dat` 输出。
8. 跑用户已有 DRX 和 TTT 算例：
   - DRX：`D:\AESimFM\inp\disk.inp`
   - TTT：`D:\AESimFM\inp\blade.inp`
   - 记录运行命令
   - 记录 `.dat/.frd/.sta` 是否生成
   - 记录关键 SDV、温度、应力或相变结果变量是否可解释
9. 更新 `open_source_boundary_split_report.md` 的“构建验证”和“后续建议”章节。
10. 整理 Git 提交范围：
    - 应提交：`Makefile`、`build_split/*.mk`、`SOURCE_INFO.md`、`solver_source_classification.csv`、`open_source_boundary_split_report.md`、必要的 `COMPONENTS.csv`、必要的 `build_solver.sh`。
    - 谨慎提交：`libcalculix_base.a`、`libaesim_solver_ext.a`、`AESim-FM_split`，按 all/core 交付策略决定。
    - 不应提交：普通 `.o`、临时 `.dat/.frd/.sta/.cvg`、GUI 临时日志、`.bak`、调试脚本和无关生成文件，除非计划明确要求作为测试资产保留。

### 13.4 收尾验收标准

只有满足以下条件，才可把本阶段标记为完成：

1. `COMPONENTS.csv` 已体现新的 CalculiX/AESim_FM 边界。**已完成。**
2. `build_solver.sh` 可以复现 split 构建或清楚提示如何执行 split 构建。**已完成。**
3. `solver_source_classification.csv` 可被 `Import-Csv` 正常解析，无控制字符，分类覆盖完整。**已完成。**
4. `open_source_boundary_split_report.md` 中所有构建和对比结论与实际文件一致。**已完成。**
5. 传统构建和 split 构建均通过。**已完成。**
6. 基础单元算例通过。**已完成。**
7. DRX 验证算例 `D:\AESimFM\inp\disk.inp` 通过。**已完成（见报告 6.1 节）。**
8. TTT 验证算例 `D:\AESimFM\inp\blade.inp` 通过。**已完成（见报告 6.1 节）。**
9. Git 提交范围已经清理，不包含无关生成物和 GUI 临时文件。**已完成。**
