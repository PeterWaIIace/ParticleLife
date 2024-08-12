import cv2
import os
import sys
import tqdm

if __name__=="__main__":

    if len(sys.argv) != 3:
        print("Usage: python script_name.py input_directory output_video_name.mp4")
        exit()

    input_directory = sys.argv[1]
    output_video = sys.argv[2]
    # Path to the directory containing PNG files
    input_directory = f"{input_directory}/"

    # Output video file name
    output_video = f"{output_video}.mp4"

    # Get the list of PNG files in the directory
    png_files = [f for f in os.listdir(input_directory) if f.endswith(".png")]

    # Sort the files based on their names (assuming they are named sequentially)
    png_files.sort()

    # Get the first PNG file to initialize video dimensions
    first_frame = cv2.imread(os.path.join(input_directory, png_files[0]))
    height, width, layers = first_frame.shape

    # Define the codec and create a VideoWriter object
    fourcc = cv2.VideoWriter_fourcc(*'mp4v')  # Codec for MP4 format
    video = cv2.VideoWriter(output_video, fourcc, 30, (width, height))

    # Iterate through PNG files and add them to the video
    for png_file in tqdm.tqdm(png_files):
        image_path = os.path.join(input_directory, png_file)
        frame = cv2.imread(image_path)
        video.write(frame)

    # Release the VideoWriter object and close the OpenCV windows
    video.release()
    cv2.destroyAllWindows()

    print(f"Video '{output_video}' created successfully.")
