      SUBROUTINE PhaseTransition_CCT(STATEV1,temp1,temp2,
     &time1,time2,cphase,phase_inf,pphase,phaseother)
        IMPLICIT NONE

        INTEGER phase_inf(4)
        REAL*8 STATEV1(phase_inf(1))
        REAL*8 temp1, temp2, time1, time2
        REAL*8 cphase(12+phase_inf(4),phase_inf(1))
        REAL*8 pphase(2,phase_inf(3),7+phase_inf(4),phase_inf(1))
        REAL*8 phaseother(15+phase_inf(1)*phase_inf(4))

        REAL*8 v_cooling, dtime, Residual
        REAL*8 v_crit_B, v_crit_P, v_crit_M
        REAL*8 Ts, Tf, Vmax, V_ii, V_i, V_inc
        REAL*8 sum_products, scale
        INTEGER NSTATV1, phase_id, npts, NCurveMax
        LOGICAL active_F, active_P, active_B, active_M

        Residual = 1.0d-5
        NSTATV1 = phase_inf(1)
        NCurveMax = phase_inf(2)

        IF(STATEV1(1).le.Residual) RETURN

        dtime = time2 - time1
        IF(dtime.le.1.0d-15) THEN
            v_cooling = 0.0d0
        ELSE
            v_cooling = ABS((temp2 - temp1) / dtime)
        ENDIF

        v_crit_B = phaseother(9)
        v_crit_P = phaseother(10)
        v_crit_M = phaseother(11)

        IF(v_cooling .lt. v_crit_B) THEN
            active_F = .TRUE.
            active_P = .TRUE.
            active_B = .FALSE.
            active_M = .FALSE.
        ELSEIF(v_cooling .lt. v_crit_P) THEN
            active_F = .TRUE.
            active_P = .TRUE.
            active_B = .TRUE.
            active_M = .FALSE.
        ELSEIF(v_cooling .lt. v_crit_M) THEN
            active_F = .FALSE.
            active_P = .TRUE.
            active_B = .TRUE.
            active_M = .FALSE.
        ELSE
            active_F = .FALSE.
            active_P = .FALSE.
            active_B = .TRUE.
            active_M = .TRUE.
        ENDIF

        DO 200 phase_id = 2, NSTATV1
            IF(phase_id.eq.2 .and. .not.active_F) GOTO 200
            IF(phase_id.eq.3 .and. .not.active_P) GOTO 200
            IF(phase_id.eq.4 .and. .not.active_B) GOTO 200
            IF(phase_id.eq.5 .and. .not.active_M) GOTO 200

            npts = NINT(cphase(5+1,phase_id))
            IF(npts.le.1) GOTO 200

            CALL CCT_Interp(pphase(1,1,1,phase_id),npts,
     &                      NCurveMax,v_cooling,Ts)
            CALL CCT_Interp(pphase(1,1,2,phase_id),npts,
     &                      NCurveMax,v_cooling,Tf)
            CALL CCT_Interp(pphase(1,1,3,phase_id),npts,
     &                      NCurveMax,v_cooling,Vmax)

            Vmax = Vmax / 100.0d0

            IF(Ts.le.0.0d0 .or. Tf.le.0.0d0) GOTO 200
            IF(Vmax.le.Residual) GOTO 200
            IF(Ts - Tf .le. Residual) GOTO 200

            IF(temp2 .gt. Tf .and. temp2 .lt. Ts) THEN
                V_ii = Vmax * (Ts - temp2) / (Ts - Tf)
            ELSEIF(temp2 .le. Tf) THEN
                V_ii = Vmax
            ELSE
                V_ii = 0.0d0
            ENDIF

            IF(temp1 .gt. Tf .and. temp1 .lt. Ts) THEN
                V_i = Vmax * (Ts - temp1) / (Ts - Tf)
            ELSEIF(temp1 .le. Tf) THEN
                V_i = Vmax
            ELSE
                V_i = 0.0d0
            ENDIF

            V_inc = V_ii - V_i
            IF(V_inc .gt. 0.0d0) THEN
                STATEV1(phase_id) = STATEV1(phase_id) + V_inc
            ENDIF
  200    CONTINUE

        sum_products = STATEV1(2) + STATEV1(3) + STATEV1(4)
     &               + STATEV1(5)
        IF(sum_products .gt. 1.0d0) THEN
            scale = 1.0d0 / sum_products
            STATEV1(2) = STATEV1(2) * scale
            STATEV1(3) = STATEV1(3) * scale
            STATEV1(4) = STATEV1(4) * scale
            STATEV1(5) = STATEV1(5) * scale
            STATEV1(1) = 0.0d0
        ELSE
            STATEV1(1) = 1.0d0 - sum_products
        ENDIF
        IF(STATEV1(1).lt.Residual) STATEV1(1) = Residual

        RETURN
        END

      SUBROUTINE CCT_Interp(table, npts, maxpts, x, y)
        IMPLICIT NONE
        INTEGER npts, maxpts, i
        REAL*8 table(2,maxpts), x, y

        IF(x .le. table(1,1)) THEN
            y = table(2,1)
            RETURN
        ENDIF
        IF(x .ge. table(1,npts)) THEN
            y = table(2,npts)
            RETURN
        ENDIF

        DO 300 i = 1, npts-1
            IF(x .ge. table(1,i) .and. x .le. table(1,i+1)) THEN
                y = table(2,i) + (x - table(1,i)) *
     &              (table(2,i+1) - table(2,i)) /
     &              (table(1,i+1) - table(1,i))
                RETURN
            ENDIF
  300    CONTINUE

        y = table(2,npts)
        RETURN
        END
