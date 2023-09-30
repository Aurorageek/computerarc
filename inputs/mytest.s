.text
.globl main

main:
    # 设置一些寄存器的初始值
    addiu $t0, $zero, 5        # $t0 = 5
    addiu $t1, $t0, 300        # $t1 = $t0 + 300
    addiu $t2, $zero, 500      # $t2 = 500
    addiu $t3, $t2, 34         # $t3 = $t2 + 34
    addiu $t3, $t3, 45         # $t3 = $t3 + 45

    # 执行一些计算，例如乘法和除法
    mult $t0, $t1             # 乘法：HI = $t0 * $t1，LO = 低32位
    mfhi $s0                  # 将HI寄存器的值存入$s0
    mflo $s1                  # 将LO寄存器的值存入$s1

    # 在$s0和$s1中保存的是$0 * 305，所以$s0应该为0，$s1应该为0
    # 接下来，我们将尝试设置HI和LO寄存器的值
    li $t4, 123456789         # 设置$t4为一个测试值

    mthi $t4                  # 设置HI寄存器的值为$t4
    mtlo $zero                # 设置LO寄存器的值为0

    # 现在HI应该为$t4的值，LO应该为0
    mfhi $s2                  # 将HI寄存器的值存入$s2
    mflo $s3                  # 将LO寄存器的值存入$s3

    # 此时，$s2应该为$t4的值，$s3应该为0

    # 在此处添加你的系统调用或其他操作

    # 退出程序
    li $v0, 10                 # 退出系统调用代码
    syscall                    # 执行系统调用

