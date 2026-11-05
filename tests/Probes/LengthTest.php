<?php

declare(strict_types=1);

// Sites harfbuzz-utf8-length, harfbuzz-language-length and harfbuzz-num-features (outcome 1): HarfBuzz reads
// the length the caller gives. A length past the string, or a feature count past the list, makes it read
// past the end of PHP's memory.

beforeEach(fn () => requireProbeBuild());

it('refuses a text_length past the string, which HarfBuzz reads past (SIGSEGV) without the refusal', function (): void {
    requireGuardPages();
    $code = <<<'PHP'
        hb_buffer_add_utf8($buffer, 'abcd', 1 << 20, 0, -1);

        return hb_buffer_get_length($buffer);
    PHP;

    $kept = hbChild($code);
    $off = hbChild($code, 'harfbuzz-utf8-length');

    expect($kept['signal'])->toBeNull()
        ->and($kept['threw'])->toBe('ValueError: hb_buffer_add_utf8(): Argument #3 ($text_length) must be -1 or not larger than the length of argument #2 ($text), and the item must lie inside the text')
        ->and($off['signal'])->toBeIn([PROBE_SIGSEGV, PROBE_SIGBUS]);
});

it('refuses an item_length past the text, which HarfBuzz 8.3 reads past (SIGSEGV) without the refusal', function (): void {
    $code = <<<'PHP'
        hb_buffer_add_utf8($buffer, 'abc', 3, 0, 1 << 24);

        return hb_buffer_get_length($buffer);
    PHP;

    $kept = hbChild($code);

    expect($kept['signal'])->toBeNull()
        ->and($kept['threw'])->toBe('ValueError: hb_buffer_add_utf8(): Argument #3 ($text_length) must be -1 or not larger than the length of argument #2 ($text), and the item must lie inside the text');
    // HarfBuzz 9 and newer clamp the item to the text, so the crash with the check off shows on 8.3 only.
    if (str_starts_with(hbVersion(), '8.')) {
        expect(hbChild($code, 'harfbuzz-utf8-length')['signal'])->toBeIn([PROBE_SIGSEGV, PROBE_SIGBUS]);
    }
});

it('refuses an item_offset past the text, which HarfBuzz 8.3 reads from outside the string', function (): void {
    $kept = hbChild("hb_buffer_add_utf8(\$buffer, 'abc', 3, 1 << 24, 1);\nreturn hb_buffer_get_length(\$buffer);");

    expect($kept['threw'])->toBe('ValueError: hb_buffer_add_utf8(): Argument #3 ($text_length) must be -1 or not larger than the length of argument #2 ($text), and the item must lie inside the text');
});

it('passes text_length -1, which HarfBuzz reads up to the terminating NUL', function (): void {
    expect(hbChild("hb_buffer_add_utf8(\$buffer, 'abcd', -1, 0, -1);\nreturn hb_buffer_get_length(\$buffer);")['result'])->toBe(4);
});

it('refuses a language len past the string, which HarfBuzz reads past (SIGSEGV) without the refusal', function (): void {
    requireGuardPages();
    $code = "return hb_language_from_string('en', 1 << 20)->pointer();";

    $kept = hbChild($code);
    $off = hbChild($code, 'harfbuzz-language-length');

    expect($kept['threw'])->toBe('ValueError: hb_language_from_string(): Argument #2 ($len) must not be larger than the length of argument #1 ($str)')
        ->and($off['signal'])->toBeIn([PROBE_SIGSEGV, PROBE_SIGBUS]);
});

it('refuses num_features past the list, which HarfBuzz reads past (SIGSEGV) without the refusal', function (): void {
    $code = <<<'PHP'
        hb_buffer_add_utf8($buffer, 'ab', -1, 0, -1);
        hb_buffer_guess_segment_properties($buffer);
        hb_shape($font, $buffer, null, 3);

        return hb_buffer_get_length($buffer);
    PHP;

    $kept = hbChild($code);
    $off = hbChild($code, 'harfbuzz-num-features');

    expect($kept['threw'])->toBe('ValueError: hb_shape(): Argument #4 ($num_features) must not be larger than the number of elements in argument #3 ($features)')
        ->and($off['signal'])->toBeIn([PROBE_SIGSEGV, PROBE_SIGBUS]);
});
