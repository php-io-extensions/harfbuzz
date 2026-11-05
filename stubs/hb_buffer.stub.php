<?php

/** @generate-class-entries */

/**
 * An hb_buffer_t. One PHP object per native pointer; dropping it does not call hb_buffer_destroy().
 * fromPointer() trusts any address other than 0.
 */
final class hb_buffer_t
{
    private function __construct() {}

    public function pointer(): int {}

    public static function fromPointer(int $pointer): static {}
}

/** An hb_language_t. HarfBuzz interns languages for the process: there is no destroy call. */
final class hb_language_t
{
    private function __construct() {}

    public function pointer(): int {}

    public static function fromPointer(int $pointer): static {}
}

/** var1 and var2 are HarfBuzz's private fields and are not read. */
final class hb_glyph_info_t
{
    public int $codepoint = 0;
    public int $mask = 0;
    public int $cluster = 0;
}

/** var is HarfBuzz's private field and is not read. */
final class hb_glyph_position_t
{
    public int $x_advance = 0;
    public int $y_advance = 0;
    public int $x_offset = 0;
    public int $y_offset = 0;
}

function hb_buffer_create(): hb_buffer_t {}

function hb_buffer_destroy(hb_buffer_t $buffer): void {}

function hb_buffer_reset(hb_buffer_t $buffer): void {}

function hb_buffer_add_utf8(hb_buffer_t $buffer, string $text, int $text_length, int $item_offset, int $item_length): void {}

function hb_buffer_guess_segment_properties(hb_buffer_t $buffer): void {}

function hb_buffer_set_direction(hb_buffer_t $buffer, int $direction): void {}

function hb_buffer_set_script(hb_buffer_t $buffer, int $script): void {}

function hb_buffer_set_language(hb_buffer_t $buffer, hb_language_t $language): void {}

function hb_language_from_string(string $str, int $len): hb_language_t {}

function hb_buffer_set_cluster_level(hb_buffer_t $buffer, int $cluster_level): void {}

function hb_buffer_get_length(hb_buffer_t $buffer): int {}

/** A copy of each hb_glyph_info_t; $length is the count HarfBuzz writes. */
function hb_buffer_get_glyph_infos(hb_buffer_t $buffer, ?int &$length): array {}

/** A copy of each hb_glyph_position_t; $length is the count HarfBuzz writes. */
function hb_buffer_get_glyph_positions(hb_buffer_t $buffer, ?int &$length): array {}
