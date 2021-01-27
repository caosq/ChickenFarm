
#if MB_LINUX_ENABLED
    ret = poll(fds, 1, timeout_ms);

#elif MB_UCOSIII_ENABLED
	(void)OSSemPend(&psMBPort->sMBWaitFinishSem, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
	(void)OSSemSet(&psMBPort->sMBWaitFinishSem, 0, &err);
#endif