<?php

declare(strict_types=1);

// A memory face's bytes must live as long as the native face, which hb-ft references: after FT_Done_Face
// (FreeType only drops a reference) and after PHP drops every object but the font. Each case runs in a
// child under libgmalloc on macOS, so a read of freed bytes faults.

it('shapes with a memory face after FT_Done_Face, which only drops a reference while hb-ft holds one', function (): void {
    $run = hbChild(sprintf(<<<'PHP'
        FT_Init_FreeType($lib);
        $bytes = file_get_contents('%s');
        FT_New_Memory_Face($lib, $bytes, strlen($bytes), 0, $memoryFace);
        FT_Set_Pixel_Sizes($memoryFace, 0, 32);
        unset($bytes);
        $shaper = hb_ft_font_create_referenced($memoryFace);
        FT_Done_Face($memoryFace);
        gc_collect_cycles();
        $text = hb_buffer_create();
        hb_buffer_add_utf8($text, 'abc', -1, 0, -1);
        hb_buffer_guess_segment_properties($text);
        hb_shape($shaper, $text, null, 0);

        return hb_buffer_get_length($text);
    PHP, monoFont()));

    expect($run['signal'])->toBeNull()
        ->and($run['result'])->toBe(3);
});

it('shapes with a memory face after PHP dropped the library, face and string objects', function (): void {
    $run = hbChild(sprintf(<<<'PHP'
        $open = static function (): hb_font_t {
            FT_Init_FreeType($lib);
            $bytes = file_get_contents('%s');
            FT_New_Memory_Face($lib, $bytes, strlen($bytes), 0, $memoryFace);
            FT_Set_Pixel_Sizes($memoryFace, 0, 32);

            return hb_ft_font_create_referenced($memoryFace);
        };
        $shaper = $open();
        gc_collect_cycles();
        $text = hb_buffer_create();
        hb_buffer_add_utf8($text, 'abc', -1, 0, -1);
        hb_buffer_guess_segment_properties($text);
        hb_shape($shaper, $text, null, 0);

        return hb_buffer_get_length($text);
    PHP, monoFont()));

    expect($run['signal'])->toBeNull()
        ->and($run['result'])->toBe(3);
});
