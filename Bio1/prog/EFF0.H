/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EFF0.H
 */

#ifndef EFF0_H
#define EFF0_H

#include "TYPES.H"

/*
    empty function used for the unused behavior slots
 */
VOID Esp_nop(VOID);

/*
    behavior that does nothing
 */
VOID Esp_00(VOID);

/*
    clear the frame delay of the effect so its sprite advances on the next animation step
 */
VOID Esp_03(VOID);

/*
    count Free2 down; once it has expired advance to the next animation step
 */
VOID Esp_0d(VOID);

/*
    toggle the hidden flag of the effect
 */
VOID Esp_0e(VOID);

/*
    advance to the next animation step and lower the vertical velocity by 0x18
 */
VOID Esp_14(VOID);

/*
    advance the sprite to the next frame entry and clear the frame delay
 */
VOID Esp_1b(VOID);

/*
    load the next animation step record over the start of the effect, keeping its DirY and scale;
    end the effect when the record has no pre behavior
 */
VOID Esp_1c(VOID);

/*
    advance the sprite by 0 to 2 frame entries at random and reload its delay and texture
    coordinates
 */
VOID Esp_1d(VOID);

/*
    select the sprite frame given by Free0 and reload its delay and texture coordinates
 */
VOID Esp_1f(VOID);

/*
    once Free2 has expired create the effect named by the effect type and group at the local
    offset, then advance to the next animation step
 */
VOID Esp_0c(VOID);

/*
    once Free0 has expired set pre behavior 1; when a free entry exists also initialise it as a
    copy of this effect at the following animation step
 */
VOID Esp_12(VOID);

/*
    create the effect named by Free0 and Free1 at the current position; the new entry number is
    left in the scratchpad
 */
VOID Esp_1a(VOID);

/*
    create the effect named by Free0 and Free1 at the current position, then advance to the next
    animation step
 */
VOID Esp_1e(VOID);

/*
    when Free3 reaches zero advance to the next animation step and copy the parent matrix,
    otherwise count Free3 down
 */
VOID Esp_02(VOID);

/*
    set the frame, then take one of the next two animation step records at random, adding its
    DirY to the current DirY and keeping Free3 and scale; copy the parent matrix
 */
VOID Esp_0b(VOID);

/*
    set post behavior 1 and matrix type 2, copy the parent matrix and set the scale from a table
    indexed by Free0
 */
VOID Esp_26(VOID);

/*
    run the timer refresh behavior, then store the player's status flags in Free2
 */
VOID Esp_28(VOID);

/*
    once Free3 has expired advance to the next animation step, set scale 0x1E and shift the
    local x offset by -0xFA
 */
VOID Esp_04(VOID);

/*
    once Free3 has expired advance to the next animation step
 */
VOID Esp_06(VOID);

/*
    when the position touches room collision (radius 4) stop the effect and set pre behavior 1
 */
VOID Esp_05(VOID);

/*
    on touching room collision (radius 4) set pre behavior 1, Free0 2 and Free2 1, reverse the x
    velocity and play a sound when Free3 equals the player's behavior flags minus 2
 */
VOID Esp_08(VOID);

/*
    when the next position reaches y 0 stop: pre and post behavior 1, draw flags set,
    acceleration and velocity cleared, displacement set to rest at ground level
 */
VOID Esp_07(VOID);

/*
    set draw flags 0x4003; when the next position falls below the player's floor level set pre
    behavior 0x2F with no post behavior and rest the effect on the floor
 */
VOID Esp_09(VOID);

/*
    shell casing landing: on reaching the floor (or the top of the first object model while
    status flag 1 is set) either end it and create a landing effect, or take a random bounce
    record, reverse the x velocity when Free2 is set and play a sound when Free3 equals the
    player's behavior flags minus 2
 */
VOID Esp_0a(VOID);

/*
    when the next position reaches y 0 end the effect (pre behavior 0x2F, no post behavior),
    rest it at ground level, create the effect named in the header and adjust the new scale by
    this scale minus 3
 */
VOID Esp_0f(VOID);

/*
    when the next position reaches y 0 set pre behavior 0x2F and draw flags 0x4007, clear
    acceleration and velocity and rest the effect at ground level
 */
VOID Esp_13(VOID);

/*
    when the next position reaches y 0 clear acceleration and velocity, rest the effect at
    ground level, reset the frame and advance to the next animation step
 */
VOID Esp_20(VOID);

/*
    when the next position reaches y 0 set pre behavior 0x2F, no post behavior and draw flags 6,
    clear acceleration and velocity and rest the effect at ground level
 */
VOID Esp_32(VOID);

/*
    while Free3 counts down randomise the blend flag, x velocity, upward acceleration, scale,
    DirY and frame; when it expires set pre behavior 0x11, type 2 and copy the parent matrix
 */
VOID Esp_10(VOID);

/*
    reverse the x acceleration on touching room collision (radius 0x3C) or while not rising;
    when the vertical velocity drops below Free3 relaunch with randomised velocity and
    acceleration and reverse Free2
 */
VOID Esp_11(VOID);

/*
    when Free0 has counted out set pre behavior 0x16, clear the vertical velocity and take the
    vertical acceleration from Free2; then test for a ground splat
 */
VOID Esp_15(VOID);

/*
    turn the effect into a ground splat when it touches room collision (radius 4) or the
    player's hit circle (radius 0x258)
 */
VOID Esp_16(VOID);

/*
    when the next position reaches y 0 turn the effect into a ground splat and set draw flags
    0x4007
 */
VOID Esp_17(VOID);

/*
    turn the effect into a ground splat: select sprite frame 5, set both behaviors to 1, clear
    velocity and acceleration and, when Free1 is set, create the splash effect (type 9) and play
    a sound
 */
VOID Esp_splat_set(VOID);

/*
    add a random 0 to 4 to the velocity and acceleration components selected by the bits of
    Free3
 */
VOID Esp_18(VOID);

/*
    add a random 0 to 2 to the blend mode bits of the draw flags and take the pre behavior from
    Free2
 */
VOID Esp_29(VOID);

/*
    set each displacement component to plus or minus 5 or 15 at random
 */
VOID Esp_36(VOID);

/*
    shift the local offset by twice the signed bytes Free0 to Free2 and advance to the next
    animation step
 */
VOID Esp_19(VOID);

/*
    select the sprite frame, then advance to the next animation step
 */
VOID Esp_21(VOID);

/*
    with the player's status flags temporarily set to Free2, test room collision (radius 4) at
    the position; on a hit create the effect named in the header and end the effect
 */
VOID Esp_22(VOID);

/*
    with the player's status flags temporarily set to Free2, run weapon hit test 6 at the
    position; on a hit create the effect named in the header and end the effect
 */
VOID Esp_25(VOID);

/*
    as Esp_10 with smaller horizontal velocity and stronger upward acceleration
 */
VOID Esp_23(VOID);

/*
    as Esp_11 with a different launch velocity and acceleration
 */
VOID Esp_24(VOID);

/*
    count Free2 and Free3 down; end the effect when Free3 reaches zero, and each time Free2
    reaches zero reset it to 4 and create the effect named by Free0 and Free1
 */
VOID Esp_27(VOID);

/*
    three-step sequence on Free3: store the player's status and behavior flags in Free0 and
    Free1, switch to type 2 with a copy of the parent matrix, then create effect type 8 group 1
    each frame
 */
VOID Esp_2a(VOID);

/*
    bullet flight: impact when below the floor or touching room collision (radius 4), or when
    weapon hit test Free1 hits; the player's status flags are set to a height layer from the
    next position during the tests
 */
VOID Esp_2b(VOID);

/*
    emit effect type 1 over three calls selected by Free0: group Id + 1, then + 2 and + 3,
    then + 4, ending the effect after the last
 */
VOID Esp_2c(VOID);

/*
    rocket flight: create the effect named in the header, end the rocket's effects when the x or
    z position goes negative, explode on room collision (radius 4); on weapon hit test 0xA play
    the explosion sounds and end the rocket's effects
 */
VOID Esp_2d(VOID);

/*
    count Free2 down and end the effect once it has expired
 */
VOID Esp_2e(VOID);

/*
    end the current effect: count its entry as free and clear both behaviors
 */
VOID Esp_2f(VOID);

/*
    toggle the hidden flag each frame and end the effect after Free1 frames
 */
VOID Esp_31(VOID);

/*
    create the sounds and hit effects of a bullet impact for the weapon in Free1 (7, 8 or 9),
    the visual effects only when Sca_hit is set, then end the effect
 */
VOID Esp_impact_set(UCHAR Sca_hit);

/*
    when system flag 0 is set set pre behavior 1 and select the sprite frame
 */
VOID Esp_33(VOID);

/*
    clear the frame delay and shrink the scale by 2 until it reaches zero
 */
VOID Esp_30(VOID);

/*
    copy the parent matrix, set type 2 and pre behavior 5, then run the settle behavior
 */
VOID Esp_34(VOID);

/*
    when the next position reaches y 0 set pre behavior 1 with no post behavior, clear
    acceleration and velocity and rest the effect at ground level
 */
VOID Esp_35(VOID);

/*
    rocket explosion: play the explosion sounds, create two explosion effects and a smoke
    effect, and end the effects belonging to the rocket
 */
VOID Esp_rocket_set(VOID);

/*
    count Free0 down and each time it expires select sprite frame 7; turn the effect into a
    ground splat on reaching y 0, or on touching the player's hit circle (radius 0x384) while
    the player is in state 1
 */
VOID Esp_37(INT unused_arg);

/*
    flicker sequence on Free0: show the flame for 11 frames, hide it, wait a random 100 to 140
    frames, repeat
 */
VOID Esp_38(VOID);

/*
    muzzle flash sequence on Free0: take a copy of the parent matrix, then count Free1 up to 9
    and loop back to sprite frame 4 until the Free2 repeats are used up
 */
VOID Esp_39(VOID);

/*
    run the timer refresh behavior, then store the low 2 bits of the auto-aim test result in
    Free2
 */
VOID Esp_3a(VOID);

/*
    once Free0 has expired create shot effect type 5 group 3 offset by Free2 and Free3, preceded
    by one at the opposite x offset when Free1 is set, tagging each with the player's weapon
    index, then advance to the next animation step
 */
VOID Esp_3b(VOID);

#endif      // EFF0_H
