import UIKit
import CoreGraphics
import ExcelCore

class WorksheetViewController: UIViewController, UICollectionViewDelegate, UICollectionViewDataSource {
    
    // MARK: - Properties
    
    private var collectionView: UICollectionView!
    private let worksheet: Worksheet
    private var cellSize: CGFloat = 100.0
    private var panGestureRecognizer: UIGestureRecognizer!
    private var pinchGestureRecognizer: UIGestureRecognizer!
    private var lastContentOffset: CGPoint = .zero
    private var zoomScale: CGFloat = 1.0
    
    // MARK: - Initialization
    
    init(worksheet: Worksheet) {
        self.worksheet = worksheet
        super.init(nibName: nil, bundle: nil)
        // Initialize other properties with default values
        self.lastContentOffset = .zero
        self.zoomScale = 1.0
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    // MARK: - Lifecycle Methods
    
    override func viewDidLoad() {
        super.viewDidLoad()
        setupCollectionView()
        configureGestureRecognizers()
        setupNavigationBar()
    }
    
    // MARK: - Setup Methods
    
    private func setupCollectionView() {
        let layout = UICollectionViewFlowLayout()
        layout.itemSize = CGSize(width: cellSize, height: cellSize)
        layout.minimumLineSpacing = 0
        layout.minimumInteritemSpacing = 0
        
        collectionView = UICollectionView(frame: view.bounds, collectionViewLayout: layout)
        collectionView.delegate = self
        collectionView.dataSource = self
        collectionView.register(UICollectionViewCell.self, forCellWithReuseIdentifier: "Cell")
        collectionView.backgroundColor = .white
        
        view.addSubview(collectionView)
    }
    
    private func configureGestureRecognizers() {
        panGestureRecognizer = UIPanGestureRecognizer(target: self, action: #selector(handlePanGesture(_:)))
        pinchGestureRecognizer = UIPinchGestureRecognizer(target: self, action: #selector(handlePinchGesture(_:)))
        
        collectionView.addGestureRecognizer(panGestureRecognizer)
        collectionView.addGestureRecognizer(pinchGestureRecognizer)
    }
    
    private func setupNavigationBar() {
        title = worksheet.name
        navigationItem.rightBarButtonItem = UIBarButtonItem(barButtonSystemItem: .add, target: self, action: #selector(addButtonTapped))
    }
    
    // MARK: - Gesture Handlers
    
    @objc private func handlePanGesture(_ gestureRecognizer: UIPanGestureRecognizer) {
        let translation = gestureRecognizer.translation(in: collectionView)
        let newContentOffset = CGPoint(
            x: lastContentOffset.x - translation.x,
            y: lastContentOffset.y - translation.y
        )
        collectionView.setContentOffset(newContentOffset, animated: false)
        
        if gestureRecognizer.state == .ended {
            lastContentOffset = newContentOffset
        }
    }
    
    @objc private func handlePinchGesture(_ gestureRecognizer: UIPinchGestureRecognizer) {
        guard let layout = collectionView.collectionViewLayout as? UICollectionViewFlowLayout else { return }
        
        if gestureRecognizer.state == .changed {
            let newZoomScale = zoomScale * gestureRecognizer.scale
            zoomScale = min(max(newZoomScale, 0.5), 2.0)
            
            let newCellSize = cellSize * zoomScale
            layout.itemSize = CGSize(width: newCellSize, height: newCellSize)
            layout.invalidateLayout()
            
            // Adjust content offset to maintain pinch center
            let pinchCenter = gestureRecognizer.location(in: collectionView)
            let newContentOffset = CGPoint(
                x: pinchCenter.x - (pinchCenter.x - collectionView.contentOffset.x) * (gestureRecognizer.scale),
                y: pinchCenter.y - (pinchCenter.y - collectionView.contentOffset.y) * (gestureRecognizer.scale)
            )
            collectionView.setContentOffset(newContentOffset, animated: false)
        }
        
        gestureRecognizer.scale = 1.0
    }
    
    // MARK: - UICollectionViewDataSource
    
    func numberOfSections(in collectionView: UICollectionView) -> Int {
        return worksheet.rowCount
    }
    
    func collectionView(_ collectionView: UICollectionView, numberOfItemsInSection section: Int) -> Int {
        return worksheet.columnCount
    }
    
    func collectionView(_ collectionView: UICollectionView, cellForItemAt indexPath: IndexPath) -> UICollectionViewCell {
        let cell = collectionView.dequeueReusableCell(withReuseIdentifier: "Cell", for: indexPath)
        
        // Get the cell data from the worksheet
        let cellAddress = CellAddress(row: indexPath.section, column: indexPath.item)
        let cellData = worksheet.getCell(at: cellAddress)
        
        // Configure the cell's appearance and content
        cell.backgroundColor = .white
        cell.layer.borderColor = UIColor.lightGray.cgColor
        cell.layer.borderWidth = 0.5
        
        let label = UILabel(frame: cell.contentView.bounds)
        label.text = cellData.value
        label.textAlignment = .center
        cell.contentView.addSubview(label)
        
        return cell
    }
    
    // MARK: - Actions
    
    @objc private func addButtonTapped() {
        // Implement add functionality
    }
}

// MARK: - Human Tasks

/*
Human tasks to be completed:
1. Implement cell editing functionality
2. Add support for formula evaluation
3. Implement cell formatting options
4. Add support for selecting multiple cells
5. Implement copy/paste functionality
6. Add support for inserting/deleting rows and columns
7. Implement undo/redo functionality
8. Add accessibility features for VoiceOver support
*/