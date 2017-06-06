probe kernel.function("vsx_unavailable_tm").call {
  if (execname() == "torture") {
    printf("vsx_unavailable_tm.call: cpuid: %ld tid: %ld, load_fp=%p, load_vec=%p, trap=%p, msr=%p ",
            cpu(), ns_tid(), task_current()->thread->load_fp, task_current()->thread->load_vec, $regs->trap, register("msr"));
//  if ($regs->trap == 0x700)
//    print_stack(callers(-1));
    print_regs();
  }
}
probe kernel.function("vsx_unavailable_tm").return {
  if (execname() == "torture") {
    printf("vsx_unavailable_tm.return: cpuid: %ld tid: %ld, load_fp=%p, load_vec=%p, trap=%p, msr=%p ",
            cpu(), ns_tid(), task_current()->thread->load_fp, task_current()->thread->load_vec, @entry($regs->trap), register("msr"));
//  if ($regs->trap == 0x700)
//    print_stack(callers(-1));
    print_regs();
  }
}
/*
probe kernel.function("save_all").call {
  if(execname() == "torture") {

    printf("call: load_fp=%p, trap=%p\n", task_current()->thread->load_fp, task_current()->thread->regs->trap);
    print_regs();
    %{

     asm ("ori 0,0,0;");

    %}
  } 
}
probe kernel.function("save_all").return {
  if(execname() == "torture") {
    printf("return: load_fp=%p\n", task_current()->thread->load_fp);
    print_regs();
  } 
}
*/
/*
probe kernel.function("arch_dup_task_struct").call {
  if(execname() == "torture") {

    printf("call: load_fp=%p, trap=%p\n", task_current()->thread->load_fp, task_current()->thread->regs->trap);
    print_regs();
  } 
}
*/
// tm_reclaim_task

probe kernel.function("__switch_to").call {
  if (execname() == "torture") {
    printf("__switch_to.call: cpuid: %ld tid: %ld, load_fp=%p, load_vec=%p, trap=%p, msr=%p, FP=%d, VMX=%d, VSX=%d, MSR=%d, ",
           cpu(), tid(), task_current()->thread->load_fp, task_current()->thread->load_vec, task_current()->thread->regs->trap, 
 													     register("msr"), register("msr") & 1 << 13 ? 1 : 0, 
                                                                                                             register("msr") & 1 << 25 ? 1 : 0,
                                                                                                             register("msr") & 1 << 23 ? 1 : 0,
                                                                                                             register("msr") & (1<<33|1<<34) ? (register("msr") & (1<<33|1<<34)) >> 32 : 0); 
//  if ($regs->trap == 0x700)
//    print_stack(callers(-1));
    print_regs();
  }
}
probe kernel.function("__switch_to").return {
  if (execname() == "torture") {
    printf("__switch_to.return: cpuid: %ld tid: %ld, load_fp=%p, load_vec=%p, trap=%p, msr=%p, FP=%d, VMX=%d, VSX=%d, MSR=%d, ",
           cpu(), tid(), task_current()->thread->load_fp, task_current()->thread->load_vec, task_current()->thread->regs->trap, 
 													     register("msr"), register("msr") & 1 << 13 ? 1 : 0, 
                                                                                                             register("msr") & 1 << 25 ? 1 : 0,
                                                                                                             register("msr") & 1 << 23 ? 1 : 0,
                                                                                                             register("msr") & (1<<33|1<<34) ? (register("msr") & (1<<33|1<<34)) >> 32 : 0); 
//  if ($regs->trap == 0x700)
//    print_stack(callers(-1));
    print_regs();
  }
}