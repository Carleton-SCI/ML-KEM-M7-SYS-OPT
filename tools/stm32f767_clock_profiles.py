#!/usr/bin/env python3
"""Validate benchmark clock profiles for the NUCLEO-F767ZI target.

The firmware uses an 8 MHz HSE bypass clock from the ST-LINK MCO and keeps
PLL48 at 48 MHz so RNG/USB-class peripherals remain valid while HCLK changes.
Keep this table in sync with Core/Src/main.c and the CMake MLKEM_CLOCK list.
"""

import argparse
import re
from dataclasses import dataclass
from pathlib import Path


HSE_MHZ = 8
APB1_MAX_MHZ = 54
APB2_MAX_MHZ = 108
PRESET_CLOCK_MHZ = (
    24, 30, 32, 36, 40, 48, 60, 72,
    84, 96, 108, 120, 144, 168, 192, 216,
)


@dataclass(frozen=True)
class ClockProfile:
    target_mhz: int
    voltage_scale: str
    pllm: int
    plln: int
    pllp: int
    pllq: int
    pllr: int
    ahb_div: int
    apb1_div: int
    apb2_div: int
    flash_latency: int
    overdrive: bool

    @property
    def pll_input_mhz(self):
        return HSE_MHZ / self.pllm

    @property
    def vco_mhz(self):
        return self.pll_input_mhz * self.plln

    @property
    def sysclk_mhz(self):
        return self.vco_mhz / self.pllp

    @property
    def hclk_mhz(self):
        return self.sysclk_mhz / self.ahb_div

    @property
    def pll48_mhz(self):
        return self.vco_mhz / self.pllq

    @property
    def apb1_mhz(self):
        return self.hclk_mhz / self.apb1_div

    @property
    def apb2_mhz(self):
        return self.hclk_mhz / self.apb2_div


PROFILES = {
    24: ClockProfile(24, "scale3", 4, 96, 8, 4, 2, 1, 4, 2, 0, False),
    30: ClockProfile(30, "scale1", 4, 120, 8, 5, 2, 1, 4, 2, 0, False),
    32: ClockProfile(32, "scale1", 4, 96, 6, 4, 2, 1, 4, 2, 1, False),
    36: ClockProfile(36, "scale1", 4, 72, 4, 3, 2, 1, 4, 2, 1, False),
    40: ClockProfile(40, "scale1", 4, 120, 6, 5, 2, 1, 4, 2, 1, False),
    42: ClockProfile(42, "scale1", 4, 168, 8, 7, 2, 1, 4, 2, 1, False),
    48: ClockProfile(48, "scale1", 4, 96, 4, 4, 2, 1, 4, 2, 1, False),
    54: ClockProfile(54, "scale1", 4, 216, 8, 9, 2, 1, 4, 2, 1, False),
    56: ClockProfile(56, "scale1", 4, 168, 6, 7, 2, 1, 4, 2, 1, False),
    60: ClockProfile(60, "scale1", 4, 120, 4, 5, 2, 1, 4, 2, 1, False),
    64: ClockProfile(64, "scale1", 4, 192, 6, 8, 2, 1, 4, 2, 2, False),
    72: ClockProfile(72, "scale1", 4, 72, 2, 3, 2, 1, 4, 2, 2, False),
    84: ClockProfile(84, "scale1", 4, 168, 4, 7, 2, 1, 4, 2, 2, False),
    96: ClockProfile(96, "scale1", 4, 96, 2, 4, 2, 1, 4, 2, 3, False),
    108: ClockProfile(108, "scale1", 4, 216, 4, 9, 2, 1, 4, 2, 3, False),
    120: ClockProfile(120, "scale1", 4, 120, 2, 5, 2, 1, 4, 2, 3, False),
    144: ClockProfile(144, "scale1", 4, 144, 2, 6, 2, 1, 4, 2, 4, False),
    168: ClockProfile(168, "scale1", 4, 168, 2, 7, 2, 1, 4, 2, 5, False),
    192: ClockProfile(192, "scale1", 4, 192, 2, 8, 2, 1, 4, 2, 6, True),
    216: ClockProfile(216, "scale1", 4, 216, 2, 9, 2, 1, 4, 2, 7, True),
}


def parse_clock(value):
    raw = value.strip().lower().replace(" ", "")
    if raw.endswith("mhz"):
        number = raw[:-3]
        if not number.isdigit():
            raise ValueError(f"Invalid MHz clock value: {value}")
        return int(number)
    if raw.endswith("hz"):
        number = raw[:-2]
        if not number.isdigit():
            raise ValueError(f"Invalid Hz clock value: {value}")
        hz = int(number)
        if hz % 1_000_000:
            raise ValueError("Hz clock values must be integer MHz values")
        return hz // 1_000_000
    if raw.isdigit():
        number = int(raw)
        if number >= 1_000_000:
            if number % 1_000_000:
                raise ValueError("Hz clock values must be integer MHz values")
            return number // 1_000_000
        return number
    raise ValueError(f"Invalid clock value: {value}")


def validate(profile):
    errors = []
    if not (1.0 <= profile.pll_input_mhz <= 2.0):
        errors.append("PLL input must be in the STM32F7 1-2 MHz range")
    if not (100.0 <= profile.vco_mhz <= 432.0):
        errors.append("PLL VCO must be in the STM32F7 100-432 MHz range")
    if profile.pllp not in (2, 4, 6, 8):
        errors.append("PLLP must be one of 2, 4, 6, 8")
    if not (2 <= profile.pllq <= 15):
        errors.append("PLLQ must be in the STM32F7 2-15 range")
    if profile.hclk_mhz != profile.target_mhz:
        errors.append("Derived HCLK does not match target")
    if profile.pll48_mhz != 48.0:
        errors.append("PLL48 is not 48 MHz")
    if profile.apb1_mhz > APB1_MAX_MHZ:
        errors.append("APB1 exceeds 54 MHz")
    if profile.apb2_mhz > APB2_MAX_MHZ:
        errors.append("APB2 exceeds 108 MHz")
    if profile.target_mhz > 180 and not profile.overdrive:
        errors.append("HCLK above 180 MHz requires overdrive")
    return errors


def format_profile(profile):
    overdrive = "on" if profile.overdrive else "off"
    return (
        f"{profile.target_mhz}mhz: pllm={profile.pllm} plln={profile.plln} "
        f"pllp={profile.pllp} pllq={profile.pllq} pllr={profile.pllr} "
        f"voltage={profile.voltage_scale} flash_latency={profile.flash_latency} "
        f"overdrive={overdrive} apb1={profile.apb1_mhz:g}mhz "
        f"apb2={profile.apb2_mhz:g}mhz pll48={profile.pll48_mhz:g}mhz"
    )


def check_repository_profiles():
    root = Path(__file__).resolve().parents[1]
    cmake = (root / "CMakeLists.txt").read_text(encoding="utf-8")
    firmware = (root / "Core/Src/main.c").read_text(encoding="utf-8")

    cmake_match = re.search(r"set\(MLKEM_VALID_CLOCK_MHZ\s+([^)]*)\)", cmake)
    if not cmake_match:
        raise ValueError("Could not find MLKEM_VALID_CLOCK_MHZ in CMakeLists.txt")
    cmake_clocks = tuple(int(value) for value in cmake_match.group(1).split())

    firmware_block = re.search(
        r"g_benchmark_clocks\[\]\s*=\s*\{(.*?)\n\};", firmware, re.S
    )
    if not firmware_block:
        raise ValueError("Could not find g_benchmark_clocks in Core/Src/main.c")
    firmware_clocks = tuple(
        int(value) for value in re.findall(r"\{\s*(\d+)\s*,", firmware_block.group(1))
    )

    expected = tuple(sorted(PROFILES))
    errors = []
    if cmake_clocks != expected:
        errors.append(f"CMake clocks {cmake_clocks} differ from tool clocks {expected}")
    if firmware_clocks != expected:
        errors.append(f"Firmware clocks {firmware_clocks} differ from tool clocks {expected}")
    if tuple(PRESET_CLOCK_MHZ) != tuple(value for value in expected if value not in (42, 54, 56, 64)):
        errors.append("Interactive preset clock list is not the documented 16-clock subset")
    if errors:
        raise ValueError("\n".join(errors))
    print("Repository clock profiles are synchronized.")


def main():
    parser = argparse.ArgumentParser(
        description="List or validate STM32F767 benchmark clock profiles."
    )
    parser.add_argument("--list-presets", action="store_true",
                        help="List the 16 interactive preset clocks")
    parser.add_argument("--list-valid", action="store_true",
                        help="List every valid firmware clock profile")
    parser.add_argument("--validate", metavar="CLOCK",
                        help="Validate a clock value such as 96mhz or 96000000")
    parser.add_argument("--check-repo", action="store_true",
                        help="Check that CMake and firmware profile lists match this tool")
    args = parser.parse_args()

    if args.list_presets:
        print(", ".join(f"{mhz}mhz" for mhz in PRESET_CLOCK_MHZ))

    if args.list_valid:
        print(", ".join(f"{mhz}mhz" for mhz in sorted(PROFILES)))

    if args.validate:
        mhz = parse_clock(args.validate)
        if mhz not in PROFILES:
            valid = ", ".join(f"{item}mhz" for item in sorted(PROFILES))
            raise SystemExit(f"Unsupported clock {mhz}mhz. Valid clocks: {valid}")
        profile = PROFILES[mhz]
        errors = validate(profile)
        print(format_profile(profile))
        if errors:
            for error in errors:
                print(f"ERROR: {error}")
            raise SystemExit(1)
        print("OK")

    if args.check_repo:
        check_repository_profiles()

    if not (args.list_presets or args.list_valid or args.validate or args.check_repo):
        parser.print_help()


if __name__ == "__main__":
    main()
