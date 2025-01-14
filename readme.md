## 神经形态控制循环代码说明

### 文件描述

此文件`neural_morphology_can_control_during_loop.c`实现了基于神经元和突触的简单神经网络模拟，通过用户交互修改神经元参数，并使用尖峰时间依赖性可塑性（STDP）规则更新突触权重。

### 主要功能

- **神经元初始化**：每个神经元具有初始膜电位、阈值及随机生成的权重。
- **突触初始化**：每个突触连接两个神经元，并赋予随机权重。
- **激活函数**：当神经元膜电位超过设定阈值时触发脉冲，并重置膜电位。
- **STDP学习规则**：根据前后神经元的脉冲时间差调整突触权重。
- **用户交互**：允许用户在运行过程中修改特定神经元的膜电位、阈值或添加外部输入。

### 代码结构

#### 定义与包含头文件

c

`#include <stdio.h> #include <stdlib.h> #include <math.h> #include <time.h> #define NUM_NEURONS 10 #define NUM_SYNAPSES 5`

#### 数据结构定义

- `Neuron` 结构体表示单个神经元，包含膜电位、阈值、权重数组及其输入数量。
- `Synapse` 结构体表示突触，包含前向神经元指针、后向神经元指针和突触权重。

#### 函数列表

- `initialize_neuron(Neuron* neuron, int num_inputs)`：初始化神经元。
- `free_neuron(Neuron* neuron)`：释放神经元占用的内存。
- `initialize_synapse(Synapse* synapse, Neuron* pre, Neuron* post)`：初始化突触。
- `activate(Neuron* neuron)`：检查并处理神经元是否达到阈值。
- `update_weights_stdp(Synapse* synapse, double learning_rate, int pre_spike, int post_spike, double time_diff)`：根据STDP规则更新突触权重。
- `user_input(Neuron* neurons)`：提供用户交互界面以修改神经元属性。

#### 主函数逻辑

1. 初始化随机数种子。
2. 创建并初始化指定数量的神经元和突触。
3. 进入仿真循环：
   - 提供用户交互选项。
   - 计算每个神经元的新膜电位。
   - 检查是否有神经元产生脉冲。
   - 根据脉冲情况更新突触权重。
4. 清理资源，释放动态分配的内存。

### 使用方法

1. 编译代码：
   
   bash
   
   `gcc -o neural_simulation neural_morphology_can_control_during_loop.c -lm`

2. 运行程序：
   
   bash
   
   `./neural_simulation`

### 注意事项

- 该程序中存在一些简化假设，例如所有神经元的输入数量相同，且突触连接为环形结构。
- 用户输入部分提供了简单的命令行界面，用于实时调整神经元状态。
- STDP规则中的时间常数和学习率可以根据具体需求进行调整。

### 依赖库

- C标准库 (`<stdio.h>`, `<stdlib.h>`, `<math.h>`, `<time.h>`)

### 联系方式

如果您有任何问题或建议，请联系3873636760@qq.com。
