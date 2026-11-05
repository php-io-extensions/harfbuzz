<?php

/** @generate-class-entries */

final class hb_feature_t
{
    public int $tag = 0;
    public int $value = 0;
    public int $start = 0;
    public int $end = 0;
}

/** $features is a list of hb_feature_t, or null; $num_features is passed as given. */
function hb_shape(hb_font_t $font, hb_buffer_t $buffer, ?array $features, int $num_features): void {}

/** HarfBuzz writes $feature on success and on failure. */
function hb_feature_from_string(string $str, int $len, ?hb_feature_t &$feature): bool {}
