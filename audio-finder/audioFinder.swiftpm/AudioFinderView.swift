import SwiftUI
import AVFoundation
import Speech

struct AudioFinderView: View {
    
    @State var audioURL = Bundle.main.url(forResource: "chicanery", withExtension: "mp3")
    @State var timestamps : Dictionary<String,Array<Float>>? = ["" : [0]]
    @State var audioPlayer : AVAudioPlayer?
    @State var searchedWord : String = ""
    @State var times : Array<Float>?
    
    var body: some View {
        VStack {
            Text("Instructions")
                .font(.title)
            
            List {
                Text("1. Play the audio to initialize the recognition process.")
                Text("2. When the \"Find Word\" and \"Find Next\" buttons appear, Audio Finder is ready to search!")
                Text("3. Choose any word you would like to find in the audio and press the \"Find Word\" button.")
                Text("4. Press \"Find Next\" to skip between each instance the word appears in the Audio.")
                Text("Hint: if the searched word could not be found, the \"Find Next\" button will loop the last successful result or restart the audio.")
                    .font(.caption)
            } // List
            .frame(height: 330)
            
            Text("The audio clip is from the AMC series Better Call Saul S3E5, Chuck's famous chicanery scene.")
                .font(.caption)
                .frame(height: 50)
                .foregroundColor(.gray)
                .padding(.horizontal)
            
            //Spacer()
            Divider()
                .padding(.horizontal, 30)
            
            Spacer()
                .frame(height: 30)
            
            AudioFinder()
            
            Spacer()
            
        } //VStack
        .padding(.top)
        
    } // View
} // AudioFinder

struct AudioFinderView_Preview: PreviewProvider {
    static var previews: some View {
        AudioFinderView()
    }
}
