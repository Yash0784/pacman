from PIL import Image, ImageEnhance
import sys

def convert_to_grid(input_path, output_path, cols, rows):
    try:
        # 1. Load image
        img = Image.open(input_path).convert('L')
        
        # 2. Boost Contrast (Helps with phone shadows/grey paper)
        enhancer = ImageEnhance.Contrast(img)
        img = enhancer.enhance(2.0) 
        
        # 3. Resize to your specific Grid Dimensions
        # NEAREST ensures we don't get "blurry" half-walls
        img = img.resize((cols, rows), Image.NEAREST)
        
        # 4. Generate Text Map
        with open(output_path, 'w') as f:
            for y in range(rows):
                line = ""
                for x in range(cols):
                    pixel = img.getpixel((x, y))
                    # Threshold: 128 is middle-grey. 
                    # If darker than 128, it's a Wall (#)
                    if pixel < 128:
                        line += "#"
                    else:
                        line += "."
                f.write(line + "\n")
                print(line) # Preview in terminal
        
        print(f"\nSuccess! Map saved to {output_path} ({cols}x{rows})")

    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    if len(sys.argv) < 5:
        print("Usage: python convert.py <input_image> <output_txt> <width> <height>")
        print("Example: python convert.py maze.jpg map.txt 28 31")
    else:
        file_in = sys.argv[1]
        file_out = sys.argv[2]
        w = int(sys.argv[3])
        h = int(sys.argv[4])
        convert_to_grid(file_in, file_out, w, h)