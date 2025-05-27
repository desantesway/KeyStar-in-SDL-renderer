import os
from PIL import Image

# 8K reference width
EIGHT_K_WIDTH = 7680

# Define target resolutions (16:9)
RESOLUTIONS = {
    "2160p": (3840, 2160),
    "1080p": (1920, 1080),
    "720p": (1280, 720)
}

# Supported image extensions
IMAGE_EXTS = (".png", ".jpg", ".jpeg", ".bmp", ".gif", ".webp")

def resize_and_save(image_path, output_dir, sizes):
    print(f"\nProcessing: {image_path}")
    img = Image.open(image_path)
    base_name = os.path.splitext(os.path.basename(image_path))[0]
    ext = os.path.splitext(image_path)[1]
    orig_w, orig_h = img.size
    print(f"  Original size: {orig_w}x{orig_h}")

    for label, (target_w, target_h) in sizes.items():
        scale = target_w / EIGHT_K_WIDTH
        new_w = max(1, int(orig_w * scale))
        new_h = max(1, int(orig_h * scale))
        print(f"  [{label}] Target width: {target_w}, scale: {scale:.4f}")
        print(f"    -> Resizing to: {new_w}x{new_h}")
        resized = img.resize((new_w, new_h), Image.LANCZOS)
        out_path = os.path.join(output_dir, f"{label}_{base_name[6:]}{ext}")
        resized.save(out_path)
        print(f"    -> Saved: {out_path}")

def main():
    folder = os.path.dirname(os.path.abspath(__file__))
    print(f"Scanning folder and all subdirectories: {folder}")
    found = False
    for root, dirs, files in os.walk(folder):
        print(f"Checking directory: {root}")
        for filename in files:
            if filename.lower().endswith(IMAGE_EXTS) and filename.startswith("4320p_"):
                found = True
                image_path = os.path.join(root, filename)
                print(f"Found image: {image_path}")
                resize_and_save(image_path, root, RESOLUTIONS)
            elif filename.lower().endswith(IMAGE_EXTS):
                print(f"Skipping (does not start with '8k_'): {os.path.join(root, filename)}")
    if not found:
        print("No images found in the folder or subdirectories that start with '8k_'.")

if __name__ == "__main__":
    main()